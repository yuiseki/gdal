/******************************************************************************
 *
 * Project:  NTF Translator
 * Purpose:  Handle NTF products that aren't recognised generically.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 1999, Frank Warmerdam
 *
 * SPDX-License-Identifier: MIT
 ****************************************************************************/

#include <stdarg.h>
#include "ntf.h"
#include "cpl_string.h"

/************************************************************************/
/* ==================================================================== */
/*                          NTFGenericClass                             */
/*                                                                      */
/*      The NTFGenericClass class exists to hold aggregated             */
/*      information for each type of record encountered in a set of     */
/*      NTF files, primarily the list of attributes actually            */
/*      encountered.                                                    */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                           NTFGenericClass                            */
/************************************************************************/

NTFGenericClass::NTFGenericClass()
    : nFeatureCount(0), b3D(FALSE), nAttrCount(0), papszAttrNames(nullptr),
      papszAttrFormats(nullptr), panAttrMaxWidth(nullptr),
      pabAttrMultiple(nullptr)
{
}

/************************************************************************/
/*                           ~NTFGenericClass                           */
/************************************************************************/

NTFGenericClass::~NTFGenericClass()

{
    CSLDestroy(papszAttrNames);
    CSLDestroy(papszAttrFormats);
    CPLFree(panAttrMaxWidth);
    CPLFree(pabAttrMultiple);
}

/************************************************************************/
/*                            CheckAddAttr()                            */
/*                                                                      */
/*      Check if an attribute already exists.  If not add it with       */
/*      its format.  Note we don't check for format conflicts at        */
/*      this time.                                                      */
/************************************************************************/

void NTFGenericClass::CheckAddAttr(const char *pszName, const char *pszFormat,
                                   int nWidth)

{
    if (EQUAL(pszName, "TX"))
        pszName = "TEXT";
    if (EQUAL(pszName, "FC"))
        pszName = "FEAT_CODE";

    const int iAttrOffset = CSLFindString(papszAttrNames, pszName);

    if (iAttrOffset == -1)
    {
        nAttrCount++;

        papszAttrNames = CSLAddString(papszAttrNames, pszName);
        papszAttrFormats = CSLAddString(papszAttrFormats, pszFormat);

        panAttrMaxWidth = static_cast<int *>(
            CPLRealloc(panAttrMaxWidth, sizeof(int) * nAttrCount));

        panAttrMaxWidth[nAttrCount - 1] = nWidth;

        pabAttrMultiple = static_cast<int *>(
            CPLRealloc(pabAttrMultiple, sizeof(int) * nAttrCount));

        pabAttrMultiple[nAttrCount - 1] = FALSE;
    }
    else
    {
        if (panAttrMaxWidth[iAttrOffset] < nWidth)
            panAttrMaxWidth[iAttrOffset] = nWidth;
    }
}

/************************************************************************/
/*                            SetMultiple()                             */
/*                                                                      */
/*      Mark this attribute as appearing multiple times on some         */
/*      features.                                                       */
/************************************************************************/

void NTFGenericClass::SetMultiple(const char *pszName)

{
    if (EQUAL(pszName, "TX"))
        pszName = "TEXT";
    if (EQUAL(pszName, "FC"))
        pszName = "FEAT_CODE";

    const int iAttrOffset = CSLFindString(papszAttrNames, pszName);
    if (iAttrOffset == -1)
        return;

    pabAttrMultiple[iAttrOffset] = TRUE;
}

/************************************************************************/
/*                           WorkupGeneric()                            */
/*                                                                      */
/*      Scan a whole file, in order to build up a list of attributes    */
/*      for the generic types.                                          */
/************************************************************************/

void OGRNTFDataSource::WorkupGeneric(NTFFileReader *poReader)

{
    NTFRecord **papoGroup = nullptr;

    if (poReader->GetNTFLevel() > 2)
    {
        poReader->IndexFile();
        if (CPLGetLastErrorType() == CE_Failure)
            return;
    }
    else
        poReader->Reset();

    /* ==================================================================== */
    /*      Read all record groups in the file.                             */
    /* ==================================================================== */
    while (true)
    {
        /* --------------------------------------------------------------------
         */
        /*      Read a record group */
        /* --------------------------------------------------------------------
         */
        if (poReader->GetNTFLevel() > 2)
            papoGroup = poReader->GetNextIndexedRecordGroup(papoGroup);
        else
            papoGroup = poReader->ReadRecordGroup();

        if (papoGroup == nullptr || papoGroup[0]->GetType() < 0 ||
            papoGroup[0]->GetType() >= 99)
            break;

        /* --------------------------------------------------------------------
         */
        /*      Get the class corresponding to the anchor record. */
        /* --------------------------------------------------------------------
         */
        NTFGenericClass *poClass = GetGClass(papoGroup[0]->GetType());
        char **papszFullAttList = nullptr;

        poClass->nFeatureCount++;

        /* --------------------------------------------------------------------
         */
        /*      Loop over constituent records collecting attributes. */
        /* --------------------------------------------------------------------
         */
        for (int iRec = 0; papoGroup[iRec] != nullptr; iRec++)
        {
            NTFRecord *poRecord = papoGroup[iRec];

            switch (poRecord->GetType())
            {
                case NRT_ATTREC:
                {
                    char **papszTypes, **papszValues;

                    poReader->ProcessAttRec(poRecord, nullptr, &papszTypes,
                                            &papszValues);

                    for (int iAtt = 0;
                         papszTypes != nullptr && papszTypes[iAtt] != nullptr;
                         iAtt++)
                    {
                        NTFAttDesc *poAttDesc =
                            poReader->GetAttDesc(papszTypes[iAtt]);
                        if (poAttDesc != nullptr &&
                            papszValues[iAtt] != nullptr)
                        {
                            poClass->CheckAddAttr(
                                poAttDesc->val_type, poAttDesc->finter,
                                static_cast<int>(strlen(papszValues[iAtt])));
                        }

                        if (CSLFindString(papszFullAttList, papszTypes[iAtt]) ==
                            -1)
                            papszFullAttList = CSLAddString(papszFullAttList,
                                                            papszTypes[iAtt]);
                        else if (poAttDesc != nullptr)
                            poClass->SetMultiple(poAttDesc->val_type);
                    }

                    CSLDestroy(papszTypes);
                    CSLDestroy(papszValues);
                }
                break;

                case NRT_TEXTREP:
                case NRT_NAMEPOSTN:
                    poClass->CheckAddAttr("FONT", "I4", 4);
                    poClass->CheckAddAttr("TEXT_HT", "R3,1", 3);
                    poClass->CheckAddAttr("TEXT_HT_GROUND", "R9,3", 9);
                    poClass->CheckAddAttr("TEXT_HT", "R3,1", 3);
                    poClass->CheckAddAttr("DIG_POSTN", "I1", 1);
                    poClass->CheckAddAttr("ORIENT", "R4,1", 4);
                    break;

                case NRT_NAMEREC:
                    poClass->CheckAddAttr("TEXT", "A*",
                                          atoi(poRecord->GetField(13, 14)));
                    break;

                case NRT_GEOMETRY:
                case NRT_GEOMETRY3D:
                    if (atoi(poRecord->GetField(3, 8)) != 0)
                        poClass->CheckAddAttr("GEOM_ID", "I6", 6);
                    if (poRecord->GetType() == NRT_GEOMETRY3D)
                        poClass->b3D = TRUE;
                    break;

                case NRT_POINTREC:
                case NRT_LINEREC:
                    if (poReader->GetNTFLevel() < 3)
                    {
                        NTFAttDesc *poAttDesc =
                            poReader->GetAttDesc(poRecord->GetField(9, 10));
                        if (poAttDesc != nullptr)
                            poClass->CheckAddAttr(poAttDesc->val_type,
                                                  poAttDesc->finter, 6);

                        if (!EQUAL(poRecord->GetField(17, 20), "    "))
                            poClass->CheckAddAttr("FEAT_CODE", "A4", 4);
                    }
                    break;

                default:
                    break;
            }
        }

        CSLDestroy(papszFullAttList);
    }

    if (GetOption("CACHING") != nullptr && EQUAL(GetOption("CACHING"), "OFF"))
        poReader->DestroyIndex();

    poReader->Reset();
}

/************************************************************************/
/*                        AddGenericAttributes()                        */
/************************************************************************/

static void AddGenericAttributes(NTFFileReader *poReader, NTFRecord **papoGroup,
                                 OGRFeature *poFeature)

{
    char **papszTypes = nullptr;
    char **papszValues = nullptr;

    if (!poReader->ProcessAttRecGroup(papoGroup, &papszTypes, &papszValues))
        return;

    for (int iAtt = 0; papszTypes != nullptr && papszTypes[iAtt] != nullptr;
         iAtt++)
    {
        int iField = 0;

        if (EQUAL(papszTypes[iAtt], "TX"))
            iField = poFeature->GetFieldIndex("TEXT");
        else if (EQUAL(papszTypes[iAtt], "FC"))
            iField = poFeature->GetFieldIndex("FEAT_CODE");
        else
            iField = poFeature->GetFieldIndex(papszTypes[iAtt]);

        if (iField == -1)
            continue;

        poReader->ApplyAttributeValue(poFeature, iField, papszTypes[iAtt],
                                      papszTypes, papszValues);

        /* --------------------------------------------------------------------
         */
        /*      Do we have a corresponding list field we should be */
        /*      accumulating this into? */
        /* --------------------------------------------------------------------
         */
        char szListName[128] = {};

        snprintf(szListName, sizeof(szListName), "%s_LIST",
                 poFeature->GetFieldDefnRef(iField)->GetNameRef());
        const int iListField = poFeature->GetFieldIndex(szListName);

        /* --------------------------------------------------------------------
         */
        /*      Yes, so perform processing similar to ApplyAttributeValue(), */
        /*      and append to list value. */
        /* --------------------------------------------------------------------
         */
        if (iListField != -1)
        {
            const char *pszAttLongName = nullptr;
            const char *pszAttValue = nullptr;
            const char *pszCodeDesc = nullptr;

            poReader->ProcessAttValue(papszTypes[iAtt], papszValues[iAtt],
                                      &pszAttLongName, &pszAttValue,
                                      &pszCodeDesc);

            if (poFeature->IsFieldSetAndNotNull(iListField))
            {
                poFeature->SetField(
                    iListField,
                    CPLSPrintf("%s,%s", poFeature->GetFieldAsString(iListField),
                               pszAttValue));
            }
            else
            {
                poFeature->SetField(iListField, pszAttValue);
            }
        }
    }

    CSLDestroy(papszTypes);
    CSLDestroy(papszValues);
}

/************************************************************************/
/*                        TranslateGenericNode()                        */
/************************************************************************/

static OGRFeature *TranslateGenericNode(NTFFileReader *poReader,
                                        OGRNTFLayer *poLayer,
                                        NTFRecord **papoGroup)

{
    if (CSLCount((char **)papoGroup) < 2 ||
        papoGroup[0]->GetType() != NRT_NODEREC ||
        (papoGroup[1]->GetType() != NRT_GEOMETRY &&
         papoGroup[1]->GetType() != NRT_GEOMETRY3D))
    {
        return nullptr;
    }

    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // NODE_ID
    poFeature->SetField("NODE_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // Geometry
    poFeature->SetGeometryDirectly(poReader->ProcessGeometry(papoGroup[1]));
    poFeature->SetField("GEOM_ID", papoGroup[1]->GetField(3, 8));

    // NUM_LINKS
    int nLinkCount = 0;
    if (papoGroup[0]->GetLength() > 18)
    {
        nLinkCount = atoi(papoGroup[0]->GetField(15, 18));
        if (nLinkCount > 0)
        {
            std::vector<int> anLinks(nLinkCount);

            // GEOM_ID_OF_LINK
            for (int iLink = 0; iLink < nLinkCount; iLink++)
                anLinks[iLink] = atoi(
                    papoGroup[0]->GetField(20 + iLink * 12, 25 + iLink * 12));

            poFeature->SetField("GEOM_ID_OF_LINK", nLinkCount, anLinks.data());

            // DIR
            for (int iLink = 0; iLink < nLinkCount; iLink++)
                anLinks[iLink] = atoi(
                    papoGroup[0]->GetField(19 + iLink * 12, 19 + iLink * 12));

            poFeature->SetField("DIR", nLinkCount, anLinks.data());
        }
    }

    poFeature->SetField("NUM_LINKS", nLinkCount);

    // should we add LEVEL and/or ORIENT?

    return poFeature;
}

/************************************************************************/
/*                     TranslateGenericCollection()                     */
/************************************************************************/

static OGRFeature *TranslateGenericCollection(NTFFileReader *poReader,
                                              OGRNTFLayer *poLayer,
                                              NTFRecord **papoGroup)

{
    if (CSLCount((char **)papoGroup) < 1 ||
        papoGroup[0]->GetType() != NRT_COLLECT)
        return nullptr;

    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // COLL_ID
    poFeature->SetField("COLL_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // NUM_PARTS
    int nPartCount = 0;

    if (papoGroup[0]->GetLength() >= 20)
    {
        nPartCount = atoi(papoGroup[0]->GetField(9, 12));
        if (nPartCount > 0 &&
            nPartCount - 1 <= (papoGroup[0]->GetLength() - 20) / 8)
        {
            std::vector<int> anParts(nPartCount);

            // TYPE
            for (int iPart = 0; iPart < nPartCount; iPart++)
                anParts[iPart] = atoi(
                    papoGroup[0]->GetField(13 + iPart * 8, 14 + iPart * 8));

            poFeature->SetField("TYPE", nPartCount, anParts.data());

            // ID
            for (int iPart = 0; iPart < nPartCount; iPart++)
                anParts[iPart] = atoi(
                    papoGroup[0]->GetField(15 + iPart * 8, 20 + iPart * 8));

            poFeature->SetField("ID", nPartCount, anParts.data());
        }
        else
        {
            nPartCount = 0;
        }
    }

    poFeature->SetField("NUM_PARTS", nPartCount);

    // ATTREC Attributes
    AddGenericAttributes(poReader, papoGroup, poFeature);

    return poFeature;
}

/************************************************************************/
/*                        TranslateGenericText()                        */
/************************************************************************/

static OGRFeature *TranslateGenericText(NTFFileReader *poReader,
                                        OGRNTFLayer *poLayer,
                                        NTFRecord **papoGroup)

{
    if (CSLCount((char **)papoGroup) < 2 ||
        papoGroup[0]->GetType() != NRT_TEXTREC)
        return nullptr;

    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // TEXT_ID
    poFeature->SetField("TEXT_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // Geometry
    for (int iRec = 0; papoGroup[iRec] != nullptr; iRec++)
    {
        if (papoGroup[iRec]->GetType() == NRT_GEOMETRY ||
            papoGroup[iRec]->GetType() == NRT_GEOMETRY3D)
        {
            poFeature->SetGeometryDirectly(
                poReader->ProcessGeometry(papoGroup[iRec]));
            poFeature->SetField("GEOM_ID", papoGroup[iRec]->GetField(3, 8));
            break;
        }
    }

    // ATTREC Attributes
    AddGenericAttributes(poReader, papoGroup, poFeature);

    // TEXTREP information
    for (int iRec = 0; papoGroup[iRec] != nullptr; iRec++)
    {
        NTFRecord *poRecord = papoGroup[iRec];

        if (poRecord->GetType() == NRT_TEXTREP)
        {
            poFeature->SetField("FONT", atoi(poRecord->GetField(9, 12)));
            poFeature->SetField("TEXT_HT",
                                atoi(poRecord->GetField(13, 15)) * 0.1);
            poFeature->SetField("TEXT_HT_GROUND",
                                atoi(poRecord->GetField(13, 15)) * 0.1 *
                                    poReader->GetPaperToGround());
            poFeature->SetField("DIG_POSTN", atoi(poRecord->GetField(16, 16)));
            poFeature->SetField("ORIENT",
                                atoi(poRecord->GetField(17, 20)) * 0.1);
            break;
        }
    }

    return poFeature;
}

/************************************************************************/
/*                        TranslateGenericName()                        */
/************************************************************************/

static OGRFeature *TranslateGenericName(NTFFileReader *poReader,
                                        OGRNTFLayer *poLayer,
                                        NTFRecord **papoGroup)

{
    if (CSLCount((char **)papoGroup) < 2 ||
        papoGroup[0]->GetType() != NRT_NAMEREC)
        return nullptr;

    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // NAME_ID
    poFeature->SetField("NAME_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // TEXT_CODE
    poFeature->SetField("TEXT_CODE", papoGroup[0]->GetField(8, 12));

    // TEXT
    int nNumChar = atoi(papoGroup[0]->GetField(13, 14));

    if (nNumChar > 0 && papoGroup[0]->GetLength() >= 15 + nNumChar - 1)
        poFeature->SetField("TEXT",
                            papoGroup[0]->GetField(15, 15 + nNumChar - 1));

    // Geometry
    for (int iRec = 0; papoGroup[iRec] != nullptr; iRec++)
    {
        if (papoGroup[iRec]->GetType() == NRT_GEOMETRY ||
            papoGroup[iRec]->GetType() == NRT_GEOMETRY3D)
        {
            poFeature->SetGeometryDirectly(
                poReader->ProcessGeometry(papoGroup[iRec]));
            poFeature->SetField("GEOM_ID", papoGroup[iRec]->GetField(3, 8));
            break;
        }
    }

    // ATTREC Attributes
    AddGenericAttributes(poReader, papoGroup, poFeature);

    // NAMEPOSTN information
    for (int iRec = 0; papoGroup[iRec] != nullptr; iRec++)
    {
        NTFRecord *poRecord = papoGroup[iRec];

        if (poRecord->GetType() == NRT_NAMEPOSTN)
        {
            poFeature->SetField("FONT", atoi(poRecord->GetField(3, 6)));
            poFeature->SetField("TEXT_HT",
                                atoi(poRecord->GetField(7, 9)) * 0.1);
            poFeature->SetField("TEXT_HT_GROUND",
                                atoi(poRecord->GetField(7, 9)) * 0.1 *
                                    poReader->GetPaperToGround());
            poFeature->SetField("DIG_POSTN", atoi(poRecord->GetField(10, 10)));
            poFeature->SetField("ORIENT",
                                atoi(poRecord->GetField(11, 14)) * 0.1);
            break;
        }
    }

    return poFeature;
}

/************************************************************************/
/*                       TranslateGenericPoint()                        */
/************************************************************************/

static OGRFeature *TranslateGenericPoint(NTFFileReader *poReader,
                                         OGRNTFLayer *poLayer,
                                         NTFRecord **papoGroup)

{
    if (CSLCount((char **)papoGroup) < 2 ||
        papoGroup[0]->GetType() != NRT_POINTREC ||
        (papoGroup[1]->GetType() != NRT_GEOMETRY &&
         papoGroup[1]->GetType() != NRT_GEOMETRY3D))
    {
        return nullptr;
    }

    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // POINT_ID
    poFeature->SetField("POINT_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // Geometry
    poFeature->SetGeometryDirectly(poReader->ProcessGeometry(papoGroup[1]));
    poFeature->SetField("GEOM_ID", papoGroup[1]->GetField(3, 8));

    // ATTREC Attributes
    AddGenericAttributes(poReader, papoGroup, poFeature);

    // Handle singular attribute in pre-level 3 POINTREC.
    if (poReader->GetNTFLevel() < 3)
    {
        char szValType[3];

        snprintf(szValType, sizeof(szValType), "%s",
                 papoGroup[0]->GetField(9, 10));
        if (!EQUAL(szValType, "  "))
        {
            const char *pszProcessedValue = nullptr;

            if (poReader->ProcessAttValue(szValType,
                                          papoGroup[0]->GetField(11, 16),
                                          nullptr, &pszProcessedValue, nullptr))
                poFeature->SetField(szValType, pszProcessedValue);
        }

        if (!EQUAL(papoGroup[0]->GetField(17, 20), "    "))
        {
            poFeature->SetField("FEAT_CODE", papoGroup[0]->GetField(17, 20));
        }
    }

    return poFeature;
}

/************************************************************************/
/*                        TranslateGenericLine()                        */
/************************************************************************/

static OGRFeature *TranslateGenericLine(NTFFileReader *poReader,
                                        OGRNTFLayer *poLayer,
                                        NTFRecord **papoGroup)

{
    if (CSLCount((char **)papoGroup) < 2 ||
        papoGroup[0]->GetType() != NRT_LINEREC ||
        (papoGroup[1]->GetType() != NRT_GEOMETRY &&
         papoGroup[1]->GetType() != NRT_GEOMETRY3D))
        return nullptr;

    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // LINE_ID
    poFeature->SetField("LINE_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // Geometry
    poFeature->SetGeometryDirectly(poReader->ProcessGeometry(papoGroup[1]));
    poFeature->SetField("GEOM_ID", papoGroup[1]->GetField(3, 8));

    // ATTREC Attributes
    AddGenericAttributes(poReader, papoGroup, poFeature);

    // Handle singular attribute in pre-level 3 LINEREC.
    if (poReader->GetNTFLevel() < 3)
    {
        char szValType[3] = {};

        snprintf(szValType, sizeof(szValType), "%s",
                 papoGroup[0]->GetField(9, 10));
        if (!EQUAL(szValType, "  "))
        {
            const char *pszProcessedValue = nullptr;

            if (poReader->ProcessAttValue(szValType,
                                          papoGroup[0]->GetField(11, 16),
                                          nullptr, &pszProcessedValue, nullptr))
                poFeature->SetField(szValType, pszProcessedValue);
        }

        if (!EQUAL(papoGroup[0]->GetField(17, 20), "    "))
        {
            poFeature->SetField("FEAT_CODE", papoGroup[0]->GetField(17, 20));
        }
    }

    return poFeature;
}

/************************************************************************/
/*                        TranslateGenericPoly()                        */
/************************************************************************/

static OGRFeature *TranslateGenericPoly(NTFFileReader *poReader,
                                        OGRNTFLayer *poLayer,
                                        NTFRecord **papoGroup)

{
    /* ==================================================================== */
    /*      Traditional POLYGON record groups.                              */
    /* ==================================================================== */
    if (CSLCount((char **)papoGroup) >= 2 &&
        papoGroup[0]->GetType() == NRT_POLYGON &&
        papoGroup[1]->GetType() == NRT_CHAIN)
    {
        OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

        // POLY_ID
        poFeature->SetField(0, atoi(papoGroup[0]->GetField(3, 8)));

        // NUM_PARTS
        int nNumLinks = atoi(papoGroup[1]->GetField(9, 12));

        if (nNumLinks < 0 || nNumLinks > MAX_LINK)
        {
            CPLError(CE_Failure, CPLE_AppDefined,
                     "MAX_LINK exceeded in ntf_generic.cpp.");
            return poFeature;
        }

        poFeature->SetField("NUM_PARTS", nNumLinks);

        // DIR
        int i, anList[MAX_LINK] = {0};

        for (i = 0; i < nNumLinks; i++)
            anList[i] = atoi(papoGroup[1]->GetField(19 + i * 7, 19 + i * 7));

        poFeature->SetField("DIR", nNumLinks, anList);

        // GEOM_ID_OF_LINK
        for (i = 0; i < nNumLinks; i++)
            anList[i] = atoi(papoGroup[1]->GetField(13 + i * 7, 18 + i * 7));

        poFeature->SetField("GEOM_ID_OF_LINK", nNumLinks, anList);

        // RingStart
        int nRingList = 0;
        poFeature->SetField("RingStart", 1, &nRingList);

        // ATTREC Attributes
        AddGenericAttributes(poReader, papoGroup, poFeature);

        // Read point geometry
        if (papoGroup[2] != nullptr &&
            (papoGroup[2]->GetType() == NRT_GEOMETRY ||
             papoGroup[2]->GetType() == NRT_GEOMETRY3D))
        {
            poFeature->SetGeometryDirectly(
                poReader->ProcessGeometry(papoGroup[2]));
            poFeature->SetField("GEOM_ID", papoGroup[2]->GetField(3, 8));
        }

        return poFeature;
    }

    return nullptr;
}

/************************************************************************/
/*                       TranslateGenericCPoly()                        */
/************************************************************************/

static OGRFeature *TranslateGenericCPoly(NTFFileReader *poReader,
                                         OGRNTFLayer *poLayer,
                                         NTFRecord **papoGroup)

{
    /* -------------------------------------------------------------------- */
    /*      First we do validation of the grouping.                         */
    /* -------------------------------------------------------------------- */
    if (papoGroup[0]->GetType() != NRT_CPOLY)
        return nullptr;

    if (papoGroup[1] == nullptr || (papoGroup[1]->GetType() != NRT_GEOMETRY &&
                                    papoGroup[1]->GetType() != NRT_GEOMETRY3D))
        return nullptr;

    if (papoGroup[2] != nullptr && papoGroup[2]->GetType() != NRT_ATTREC)
        return nullptr;

    /* -------------------------------------------------------------------- */
    /*      collect information for whole complex polygon.                  */
    /* -------------------------------------------------------------------- */
    OGRFeature *poFeature = new OGRFeature(poLayer->GetLayerDefn());

    // CPOLY_ID
    poFeature->SetField("CPOLY_ID", atoi(papoGroup[0]->GetField(3, 8)));

    // ATTREC Attributes
    AddGenericAttributes(poReader, papoGroup, poFeature);

    // Read point geometry
    if (papoGroup[1] != nullptr && (papoGroup[1]->GetType() == NRT_GEOMETRY ||
                                    papoGroup[1]->GetType() == NRT_GEOMETRY3D))
    {
        poFeature->SetGeometryDirectly(poReader->ProcessGeometry(papoGroup[1]));
        poFeature->SetField("GEOM_ID", atoi(papoGroup[1]->GetField(3, 8)));
    }

    /* -------------------------------------------------------------------- */
    /*      Collect the chains for each of the rings, and just aggregate    */
    /*      these into the master list without any concept of where the     */
    /*      boundaries are.  The boundary information will be emitted      */
    /*      in the RingStart field.                                         */
    /* -------------------------------------------------------------------- */
    int nNumLink = 0;
    int anPolyId[MAX_LINK * 2] = {0};

    nNumLink = atoi(papoGroup[0]->GetField(9, 12));
    if (nNumLink < 0 || nNumLink > MAX_LINK)
    {
        CPLError(CE_Failure, CPLE_AppDefined,
                 "MAX_LINK exceeded in ntf_generic.cpp.");
        return poFeature;
    }

    for (int iLink = 0; iLink < nNumLink; iLink++)
    {
        anPolyId[iLink] =
            atoi(papoGroup[0]->GetField(13 + iLink * 7, 18 + iLink * 7));
    }

    // NUM_PARTS
    poFeature->SetField("NUM_PARTS", nNumLink);

    // POLY_ID
    poFeature->SetField("POLY_ID", nNumLink, anPolyId);

    return poFeature;
}

/************************************************************************/
/*                       EstablishGenericLayers()                       */
/************************************************************************/

void OGRNTFDataSource::EstablishGenericLayers()

{
    /* -------------------------------------------------------------------- */
    /*      Pick an initial NTFFileReader to build the layers against.      */
    /* -------------------------------------------------------------------- */
    for (int iFile = 0; iFile < nNTFFileCount; iFile++)
    {
        int bHasZ = FALSE;

        NTFFileReader *poPReader = papoNTFFileReader[iFile];
        if (poPReader->GetProductId() != NPC_UNKNOWN)
            continue;

        /* --------------------------------------------------------------------
         */
        /*      If any of the generic classes are 3D, then assume all our */
        /*      geometry should be marked as 3D. */
        /* --------------------------------------------------------------------
         */
        for (int iType = 0; iType < 99; iType++)
        {
            NTFGenericClass *poClass = aoGenericClass + iType;

            if (poClass->nFeatureCount > 0 && poClass->b3D)
                bHasZ = TRUE;
        }

        /* --------------------------------------------------------------------
         */
        /*      Create layers for all recognised layer types with features. */
        /* --------------------------------------------------------------------
         */
        for (int iType = 0; iType < 99; iType++)
        {
            NTFGenericClass *poClass = aoGenericClass + iType;

            if (poClass->nFeatureCount == 0)
                continue;

            if (iType == NRT_POINTREC)
            {
                poPReader->EstablishLayer(
                    "GENERIC_POINT", OGR_GT_SetModifier(wkbPoint, bHasZ, FALSE),
                    TranslateGenericPoint, NRT_POINTREC, poClass, "POINT_ID",
                    OFTInteger, 6, 0, NULL);
            }
            else if (iType == NRT_LINEREC)
            {
                poPReader->EstablishLayer(
                    "GENERIC_LINE",
                    OGR_GT_SetModifier(wkbLineString, bHasZ, FALSE),
                    TranslateGenericLine, NRT_LINEREC, poClass, "LINE_ID",
                    OFTInteger, 6, 0, NULL);
            }
            else if (iType == NRT_TEXTREC)
            {
                poPReader->EstablishLayer(
                    "GENERIC_TEXT", OGR_GT_SetModifier(wkbPoint, bHasZ, FALSE),
                    TranslateGenericText, NRT_TEXTREC, poClass, "TEXT_ID",
                    OFTInteger, 6, 0, NULL);
            }
            else if (iType == NRT_NAMEREC)
            {
                poPReader->EstablishLayer(
                    "GENERIC_NAME", OGR_GT_SetModifier(wkbPoint, bHasZ, FALSE),
                    TranslateGenericName, NRT_NAMEREC, poClass, "NAME_ID",
                    OFTInteger, 6, 0, NULL);
            }
            else if (iType == NRT_NODEREC)
            {
                poPReader->EstablishLayer(
                    "GENERIC_NODE", OGR_GT_SetModifier(wkbPoint, bHasZ, FALSE),
                    TranslateGenericNode, NRT_NODEREC, poClass, "NODE_ID",
                    OFTInteger, 6, 0, "NUM_LINKS", OFTInteger, 4, 0,
                    "GEOM_ID_OF_LINK", OFTIntegerList, 6, 0, "DIR",
                    OFTIntegerList, 1, 0, NULL);
            }
            else if (iType == NRT_COLLECT)
            {
                poPReader->EstablishLayer(
                    "GENERIC_COLLECTION", wkbNone, TranslateGenericCollection,
                    NRT_COLLECT, poClass, "COLL_ID", OFTInteger, 6, 0,
                    "NUM_PARTS", OFTInteger, 4, 0, "TYPE", OFTIntegerList, 2, 0,
                    "ID", OFTIntegerList, 6, 0, NULL);
            }
            else if (iType == NRT_POLYGON)
            {
                poPReader->EstablishLayer(
                    "GENERIC_POLY", OGR_GT_SetModifier(wkbPoint, bHasZ, FALSE),
                    TranslateGenericPoly, NRT_POLYGON, poClass, "POLY_ID",
                    OFTInteger, 6, 0, "NUM_PARTS", OFTInteger, 4, 0, "DIR",
                    OFTIntegerList, 1, 0, "GEOM_ID_OF_LINK", OFTIntegerList, 6,
                    0, "RingStart", OFTIntegerList, 6, 0, NULL);
            }
            else if (iType == NRT_CPOLY)
            {
                poPReader->EstablishLayer(
                    "GENERIC_CPOLY", OGR_GT_SetModifier(wkbPoint, bHasZ, FALSE),
                    TranslateGenericCPoly, NRT_CPOLY, poClass, "CPOLY_ID",
                    OFTInteger, 6, 0, "NUM_PARTS", OFTInteger, 4, 0, "POLY_ID",
                    OFTIntegerList, 1, 0, NULL);
            }
        }
    }
}
