.. _vector.sqlite:

SQLite / Spatialite RDBMS
=========================

.. shortname:: SQLite

.. build_dependencies:: libsqlite3 or libspatialite

OGR optionally supports spatial and non-spatial tables stored in SQLite
3.x database files. SQLite is a "light weight" single file based RDBMS
engine with fairly complete SQL semantics and respectable performance.

The driver can handle "regular" SQLite databases, as well as Spatialite
databases (spatial enabled SQLite databases). The type of an existing
database can be checked from the SQLITE debug info value "OGR style
SQLite DB found/ SpatiaLite DB found/SpatiaLite v4 DB found" obtained by
running ``ogrinfo db.sqlite --debug on``

Starting with GDAL 2.2, the SQLite driver can also read databases with
:ref:`RasterLite2 raster coverages <raster.rasterlite2>`.

The SQLite database is essentially typeless, but the SQLite driver will
attempt to classify attributes field as text, integer or floating point
based on the contents of the first record in a table. Datetime field types
are also handled.

Starting with GDAL 2.2, the "JSonStringList", "JSonIntegerList",
"JSonInteger64List" and "JSonRealList" SQLite declaration types are used
to map the corresponding OGR StringList, IntegerList, Integer64List and
RealList types. The field values are then encoded as JSON arrays, with
proper CSV escaping.

SQLite databases often do not work well over NFS, or some other
networked file system protocols due to the poor support for locking. It
is safest to operate only on SQLite files on a physical disk of the
local system.

SQLite is an optionally compiled in driver. It is not compiled in by
default.

By default, SQL statements are passed directly to the SQLite database
engine. It's also possible to request the driver to handle SQL commands
with :ref:`OGR SQL <ogr_sql_dialect>` engine, by passing **"OGRSQL"** string
to the ExecuteSQL() method, as name of the SQL dialect.

The :config:`OGR_SQLITE_SYNCHRONOUS` configuration option
has been added. When set to OFF, this issues a 'PRAGMA synchronous =
OFF' command to the SQLite database. This has the advantage of
speeding-up some write operations (e.g. on EXT4 filesystems), but at the
expense of data safety w.r.t system/OS crashes. So use it carefully in
production environments and read the SQLite `related
documentation <http://www.sqlite.org/pragma.html#pragma_synchronous>`__.

Any SQLite
`pragma <http://www.sqlite.org/pragma.html>`__ can be specified with the
:config:`OGR_SQLITE_PRAGMA` configuration option. The syntax is
``OGR_SQLITE_PRAGMA = "pragma_name=pragma_value[,pragma_name2=pragma_value2]*"``.

Driver capabilities
-------------------

.. supports_create::

.. supports_georeferencing::

.. supports_virtualio::

"Regular" SQLite databases
--------------------------

The driver looks for a geometry_columns table laid out as defined
loosely according to OGC Simple Features standards, particularly as
defined in `FDO RFC 16 <http://trac.osgeo.org/fdo/wiki/FDORfc16>`__. If
found it is used to map tables to layers.

If geometry_columns is not found, each table is treated as a layer.
Layers with a WKT_GEOMETRY field will be treated as spatial tables, and
the WKT_GEOMETRY column will be read as Well Known Text geometry.

If geometry_columns is found, it will be used to lookup spatial
reference systems in the spatial_ref_sys table.

While the SQLite driver supports reading spatial data from records,
there is no support for spatial indexing, so spatial queries will tend
to be slow (use Spatialite for that). Attributes queries may be fast,
especially if indexes are built for appropriate attribute columns using
the "CREATE INDEX ON ( )" SQL command.

The driver also supports reading and writing the
following non-linear geometry types :CIRCULARSTRING, COMPOUNDCURVE,
CURVEPOLYGON, MULTICURVE and MULTISURFACE. Note: this is not true for
Spatialite databases, since those geometry types are not supported by
current Spatialite versions.

Tables with multiple geometry columns
-------------------------------------

Layers with multiple geometry columns can be
created, modified or read, following new API described in :ref:`rfc-41`

REGEXP operator
---------------

By default, the REGEXP operator has no implementation in SQLite. With
OGRbuilt against the PCRE library, the REGEXP operator is
available in SQL statements run by OGR.

Using the SpatiaLite library (Spatial extension for SQLite)
-----------------------------------------------------------

The SQLite driver can read and write SpatiaLite databases. Creating or
updating a spatialite database requires explicit linking against
SpatiaLite library (version >= 2.3.1). Explicit linking against
SpatiaLite library also provides access to functions provided by this
library, such as spatial indexes, spatial functions, etc...

A few examples :

::

   # Duplicate the sample database provided with SpatiaLite
   ogr2ogr -f SQLite testspatialite.sqlite test-2.3.sqlite  -dsco SPATIALITE=YES

   # Make a request with a spatial filter. Will work faster if spatial index has
   # been created and explicit linking against SpatiaLite library.
   ogrinfo testspatialite.sqlite Towns -spat 754000 4692000 770000 4924000

Opening with 'VirtualShape:'
----------------------------

(Require Spatialite support)

It is possible to open on-the-fly a shapefile as a VirtualShape with
Spatialite. The syntax to use for the datasource is
"VirtualShape:/path/to/shapefile.shp" (the shapefile must be a "real"
file).

This gives the capability to use the spatial operations of Spatialite
(note that spatial indexes on virtual tables are not available).

The SQLite SQL dialect
----------------------

The SQLite SQL engine can be used to run SQL
queries on any OGR datasource if using the :ref:`sql_sqlite_dialect`.

The VirtualOGR SQLite extension
-------------------------------

The GDAL/OGR library can be loaded as a `SQLite
extension <http://www.sqlite.org/lang_corefunc.html#load_extension>`__.
The extension is loaded with the load_extension(gdal_library_name) SQL
function, where gdal_library_name is typically libgdal.so on Unix/Linux,
gdal110.dll on Windows, etc..

After the extension is loaded, a virtual table, corresponding to a OGR
layer, can be created with one of the following SQL statement :

::

   CREATE VIRTUAL TABLE table_name USING VirtualOGR(datasource_name);
   CREATE VIRTUAL TABLE table_name USING VirtualOGR(datasource_name, update_mode);
   CREATE VIRTUAL TABLE table_name USING VirtualOGR(datasource_name, update_mode, layer_name);
   CREATE VIRTUAL TABLE table_name USING VirtualOGR(datasource_name, update_mode, layer_name, expose_ogr_style);

where :

-  *datasource_name* is the connection string to any OGR datasource.
-  *update_mode* = 0 for read-only mode (default value) or 1 for update
   mode.
-  *layer_name* = the name of a layer of the opened datasource.
-  *expose_ogr_style* = 0 to prevent the OGR_STYLE special from being
   displayed (default value) or 1 to expose it.

Note: *layer_name* does not need to be specified if the datasource has
only one single layer.

From the sqlite3 console, a typical use case is :

::

   sqlite> SELECT load_extension('libgdal.so');

   sqlite> SELECT load_extension('mod_spatialite.so');

   sqlite> CREATE VIRTUAL TABLE poly USING VirtualOGR('poly.shp');

   sqlite> SELECT *, ST_Area(GEOMETRY) FROM POLY;
   215229.266|168.0|35043411||215229.265625
   247328.172|179.0|35043423||247328.171875
   261752.781|171.0|35043414||261752.78125
   547597.188|173.0|35043416||547597.2109375
   15775.758|172.0|35043415||15775.7578125
   101429.977|169.0|35043412||101429.9765625
   268597.625|166.0|35043409||268597.625
   1634833.375|158.0|35043369||1634833.390625
   596610.313|165.0|35043408||596610.3359375
   5268.813|170.0|35043413||5268.8125

Alternatively, you can use the
*ogr_datasource_load_layers(datasource_name[, update_mode[, prefix]])*
function to automatically load all the layers of a datasource.

::

   sqlite> SELECT load_extension('libgdal.so');

   sqlite> SELECT load_extension('mod_spatialite.so');

   sqlite> SELECT ogr_datasource_load_layers('poly.shp');
   1
   sqlite> SELECT * FROM sqlite_master;
   table|poly|poly|0|CREATE VIRTUAL TABLE "poly" USING VirtualOGR('poly.shp', 0, 'poly')

Refer to the :ref:`sql_sqlite_dialect` for an
overview of the capabilities of VirtualOGR tables.

Creation Issues
---------------

The SQLite driver supports creating new SQLite database files, or adding
tables to existing ones.

Transaction support
~~~~~~~~~~~~~~~~~~~

The driver implements transactions at the database level, per :ref:`rfc-54`

Relationships
-------------

.. versionadded:: 3.6

Relationship retrieval is supported. Relationships will be reported for tables which utilize FOREIGN KEY
constraints.

Relationship creation is supported since GDAL 3.7, for one-to-many relationships only.

Dataset open options
~~~~~~~~~~~~~~~~~~~~

|about-open-options|
The following open options are supported:

-  .. oo:: LIST_ALL_TABLES
      :choices: YES, NO

      This may be "YES" to force all tables,
      including non-spatial ones, to be listed.

-  .. oo:: LIST_VIRTUAL_OGR
      :choices: YES, NO

      This may be "YES" to force VirtualOGR
      virtual tables to be listed. This should only be enabled on trusted
      datasources to avoid potential safety issues.

-  .. oo:: PRELUDE_STATEMENTS
      :since: 3.2

      SQL statement(s) to
      send on the SQLite3 connection before any other ones. In
      case of several statements, they must be separated with the
      semi-column (;) sign. This option may be useful
      to `attach another database <https://www.sqlite.org/lang_attach.html>`__
      to the current one and issue cross-database requests.

      .. note::
           The other database must be of a type recognized by this driver, so
           its geometry blobs are properly recognized (so typically not a GeoPackage one)

-  .. oo:: OGR_SCHEMA
      :choices: <filename>|<json string>
      :since: 3.11.0

      Partially or totally overrides the auto-detected schema to use for creating the layer.
      The overrides are defined as a JSON list of field definitions.
      This can be a filename, a URL or JSON string conformant with the `ogr_fields_override.schema.json schema <https://raw.githubusercontent.com/OSGeo/gdal/refs/heads/master/ogr/data/ogr_fields_override.schema.json>`_


Database creation options
~~~~~~~~~~~~~~~~~~~~~~~~~

|about-dataset-creation-options|
The following dataset creation options are supported:

-  .. dsco:: METADATA
      :choices: YES, NO

      This can be used to avoid creating the
      geometry_columns and spatial_ref_sys tables in a new database. By
      default these metadata tables are created when a new database is
      created.

-  .. dsco:: SPATIALITE
      :choices: YES, NO

      Create the
      SpatiaLite flavor of the metadata tables, which are a bit differ
      from the metadata used by this OGR driver and from OGC
      specifications. Implies :dsco:`METADATA=YES`.

      Please note: OGR must be linked against
      *libspatialite* in order to support insert/write on SpatiaLite; if
      not, *read-only* mode is enforced.

      Attempting to perform any insert/write on SpatiaLite skipping the
      appropriate library support simply produces broken (corrupted)
      DB-files.

      Important notice: when the underlying *libspatialite* is v.2.3.1
      (or any previous version) any Geometry will be casted to 2D [XY],
      because earlier versions of this library are simply able to support
      2D [XY] dimensions. Version 2.4.0 (or any subsequent) is required
      in order to support 2.5D [XYZ].

-  .. dsco:: INIT_WITH_EPSG
      :choices: YES, NO

      Insert the
      content of the EPSG CSV files into the spatial_ref_sys table.
      Defaults to NO for regular SQLite databases.
      Please note: if :dsco:`SPATIALITE=YES` and the underlying
      *libspatialite* is v2.4 or v3.X, :dsco:`INIT_WITH_EPSG` is ignored;
      those library versions will unconditionally load the EPSG dataset
      into the spatial_ref_sys table when creating a new DB
      (*self-initialization*). Starting with libspatialite 4.0,
      :dsco:`INIT_WITH_EPSG` defaults to YES, but can be set to NO.

Layer creation options
~~~~~~~~~~~~~~~~~~~~~~

|about-layer-creation-options|
The following layer creation options are supported:

-  .. lco:: FORMAT
      :choices: WKB, WKT, SPATIALITE
      :default: WKB

      Controls the format used for the
      geometry column. By default WKB (Well Known Binary) is used. This is
      generally more space and processing efficient, but harder to inspect
      or use in simple applications than WKT (Well Known Text). SpatiaLite
      extension uses its own binary format to store geometries and you can
      choose it as well. It will be selected automatically when SpatiaLite
      database is opened or created with :dsco:`SPATIALITE=YES` option.
      SPATIALITE value is available.

-  .. lco:: GEOMETRY_NAME

      By default OGR creates
      new tables with the geometry column named GEOMETRY (or WKT_GEOMETRY
      if :lco:`FORMAT=WKT`). If you wish to use a different name, it can be
      supplied with the :lco:`GEOMETRY_NAME` layer creation option.

-  .. lco:: LAUNDER
      :choices: YES, NO
      :default: YES

      Controls whether layer and field names will be
      laundered for easier use in SQLite. Laundered names will be converted
      to lower case and some special characters(' - #) will be changed to
      underscores.

-  .. lco:: SPATIAL_INDEX
      :choices: YES, NO
      :default: YES

      If the database
      is of the SpatiaLite flavor, and if OGR is linked against
      libspatialite, this option can be used to control if a spatial index
      must be created.

-  .. lco:: COMPRESS_GEOM
      :choices: YES, NO
      :default: NO

      If the format of
      the geometry BLOB is of the SpatiaLite flavor, this option can be
      used to control if the compressed format for geometries (LINESTRINGs,
      POLYGONs) must be used. This format is understood by Spatialite v2.4
      (or any subsequent version). Default to NO. Note: when updating an
      existing Spatialite DB, the :config:`COMPRESS_GEOM` configuration option
      can be set to produce similar results for appended/overwritten features.

-  .. lco:: SRID

      Used to force the SRID
      number of the SRS associated with the layer. When this option isn't
      specified and that a SRS is associated with the layer, a search is
      made in the spatial_ref_sys to find a match for the SRS, and, if
      there is no match, a new entry is inserted for the SRS in the
      spatial_ref_sys table. When the SRID option is specified, this search
      (and the eventual insertion of a new entry) will not be done : the
      specified SRID is used as such.

-  .. lco:: COMPRESS_COLUMNS
      :choices: <column_name1[\,column_name2\, ...]>

      A list of (String) columns that must be compressed with
      ZLib DEFLATE algorithm. This might be beneficial for databases that
      have big string blobs. However, use with care, since the value of
      such columns will be seen as compressed binary content with other
      SQLite utilities (or previous OGR versions). With OGR, when
      inserting, modifying or querying compressed columns,
      compression/decompression is done transparently. However, such
      columns cannot be (easily) queried with an attribute filter or WHERE
      clause. Note: in table definition, such columns have the
      "VARCHAR_deflate" declaration type.

-  .. lco:: FID
      :default: OGC_FID

      Name of the FID column to create.

-  .. lco:: STRICT
      :choices: YES, NO
      :default: NO
      :since: 3.3.5

      (SQLite >= 3.37)
      Whether the table should be created as a `strict table <https://sqlite.org/stricttables.html>`__,
      that is strong column type checking. This normally has little influence when
      operating only through OGR, since it has typed columns, but can help to
      strengthen database integrity when the database might be edited by external
      tools.
      Note that databases that contain STRICT tables can only be read by SQLite >= 3.37.
      The set of column data types supported in STRICT mode is: Integer, Integer64, Real,
      String, DateTime, Date and Time. The :lco:`COMPRESS_COLUMNS` option is ignored in
      strict mode.

Configuration options
---------------------

|about-config-options|
The following configuration options are available:

- .. config:: SQLITE_LIST_ALL_TABLES
     :choices: YES, NO
     :default: NO

     Set to "YES" to list
     all tables (not just the tables listed in the geometry_columns table). This
     can also be done using the :oo:`LIST_ALL_TABLES` open option.

- .. config:: OGR_PROMOTE_TO_INTEGER64
     :choices: YES, NO
     :default: NO

     Whether to read fields with type ``INTEGER`` as 64-bit integers.

- .. config:: OGR_SQLITE_LIST_VIRTUAL_OGR
     :choices: YES, NO
     :default: NO

     Set to "YES" to
     list VirtualOGR layers. Defaults to "NO" as there might be some security
     implications if a user is provided with a file and doesn't know that there
     are virtual OGR tables in it.

- .. config:: OGR_SQLITE_JOURNAL

     can be used to set the journal mode of the SQLite file, see also
     https://www.sqlite.org/pragma.html#pragma_journal_mode.

- .. config:: OGR_SQLITE_CACHE

     see :ref:`Performance hints <target_drivers_vector_sqlite_performance_hints>`.

- .. config:: OGR_SQLITE_SYNCHRONOUS

     see :ref:`Performance hints <target_drivers_vector_sqlite_performance_hints>`.

- .. config:: OGR_SQLITE_LOAD_EXTENSIONS
     :choices: <extension1\,...\,extensionN>, ENABLE_SQL_LOAD_EXTENSION
     :since: 3.5.0

     Comma separated list of names of shared libraries containing
     extensions to load at database opening.
     If a file cannot be loaded directly, attempts are made to load with various
     operating-system specific extensions added. So
     for example, if "samplelib" cannot be loaded, then names like "samplelib.so"
     or "samplelib.dylib" or "samplelib.dll" might be tried also.
     The special value ``ENABLE_SQL_LOAD_EXTENSION`` can be used to enable the use of
     the SQL ``load_extension()`` function, which is normally disabled in standard
     builds of sqlite3.
     Loading extensions as a potential security impact if they are untrusted.

- .. config:: OGR_SQLITE_PRAGMA

     with this option any SQLite
     `pragma <http://www.sqlite.org/pragma.html>`__ can be specified. The syntax is
     ``OGR_SQLITE_PRAGMA = "pragma_name=pragma_value[,pragma_name2=pragma_value2]*"``.

- .. config:: SQLITE_USE_OGR_VFS
     :choices: YES, NO

     YES enables extra buffering/caching
     by the GDAL/OGR I/O layer and can speed up I/O. More information
     :ref:`here <vsicached>`.
     Be aware that no file locking will occur if this option is activated, so
     concurrent edits may lead to database corruption.

- .. config:: COMPRESS_GEOM
     :choices: YES, NO
     :default: NO

     Equivalent of :lco:`COMPRESS_GEOM` layer creation option for use when
     updating or appending to an existing layer.


.. _target_drivers_vector_sqlite_performance_hints:

Performance hints
-----------------

SQLite is a Transactional DBMS; while many INSERT statements are
executed in close sequence, BEGIN TRANSACTION and COMMIT TRANSACTION
statements have to be invoked appropriately (with the
OGR_L_StartTransaction() / OGR_L_CommitTransaction()) in order to get
optimal performance. By default, if no transaction is explicitly
started, SQLite will autocommit on every statement, which will be slow.
If using ogr2ogr, its default behavior is to COMMIT a transaction every
100000 inserted rows. The **-gt** argument allows explicitly setting the
number of rows for each transaction.

SQLite usually has a very minimal memory foot-print; just about 20MB of
RAM are reserved to store the internal Page Cache [merely 2000 pages].
This value too may well be inappropriate under many circumstances, most
notably when accessing some really huge DB-file containing many tables
related to a corresponding Spatial Index. Explicitly setting a much more
generously dimensioned internal Page Cache may often help to get a
noticeably better performance. You can
explicitly set the internal Page Cache size using the configuration
option :config:`OGR_SQLITE_CACHE` *value* [*value* being
measured in MB]; if your HW has enough available RAM, defining a Cache
size as big as 512MB (or even 1024MB) may sometimes help a lot in order
to get better performance.

Setting the :config:`OGR_SQLITE_SYNCHRONOUS` configuration
option to *OFF* might also increase performance when creating SQLite
databases (although at the expense of integrity in case of
interruption/crash ).

If many source files will be collected into the same Spatialite table,
it can be much faster to initialize the table without a spatial index by
using -lco :lco:`SPATIAL_INDEX=NO` and to create spatial index with a separate
command after all the data are appended. Spatial index can be created
with ogrinfo command

::

   ogr2ogr -f SQLite -dsco SPATIALITE=YES db.sqlite first.shp -nln the_table -lco SPATIAL_INDEX=NO
   ogr2ogr -append db.sqlite second.shp -nln the_table
   ...
   ogr2ogr -append db.sqlite last.shp -nln the_table
   ogrinfo db.sqlite -sql "SELECT CreateSpatialIndex('the_table','GEOMETRY')"

If a database has gone through editing operations, it might be useful to
run a `VACUUM <https://sqlite.org/lang_vacuum.html>`__ query to compact
and optimize it.

::

   ogrinfo db.sqlite -sql "VACUUM"

Secure deletion
---------------

Depending on how SQLite3 is built, `secure deletion <https://www.sqlite.org/pragma.html#pragma_secure_delete>`__
might or might not be enabled.
Starting with GDAL 3.10, secure deletion is always enabled, unless
``SECURE_DELETE`` is specified through the :config:`OGR_SQLITE_PRAGMA`
configuration option.
Note that secure deletion does not recover potential lost space, so running
a `VACUUM <https://sqlite.org/lang_vacuum.html>`__ query is recommended to fully
optimized a database that has been subject to updates or deletions.

Example
-------

- Convert a non-spatial SQLite table into a GeoPackage:

.. code-block::

  ogr2ogr \
    -f "GPKG" output.gpkg \
    input.sqlite \
    -sql \
    "SELECT
       *,
       MakePoint(longitude, latitude, 4326) AS geometry
     FROM
       my_table" \
    -nln "location" \
    -s_srs "EPSG:4326"

- Perform a join between 2 SQLite/Spatialite databases:

.. code-block::

    ogrinfo my_spatial.db \
        -sql "SELECT poly.id, other.foo FROM poly JOIN other_schema.other USING (id)" \
        -oo PRELUDE_STATEMENTS="ATTACH DATABASE 'other.db' AS other_schema"

Credits
-------

-  Development of the OGR SQLite driver was supported by `DM Solutions
   Group <http://www.dmsolutions.ca/>`__ and
   `GoMOOS <http://www.gomoos.org/>`__.
-  Full support for SpatiaLite was contributed by A.Furieri, with
   funding from `Regione Toscana <http://www.regione.toscana.it/>`__

Links
-----

-  `http://www.sqlite.org <http://www.sqlite.org/>`__: Main SQLite page.
-  https://www.gaia-gis.it/fossil/libspatialite/index: SpatiaLite extension to SQLite.
-  `FDO RFC 16 <http://trac.osgeo.org/fdo/wiki/FDORfc16>`__: FDO
   Provider for SQLite
-  :ref:`RasterLite2 driver <raster.rasterlite2>`
