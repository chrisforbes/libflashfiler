#ifndef _FF_INTERNAL_H
#define _FF_INTERNAL_H

struct ff_db
{
	HANDLE fileHandle;
	HANDLE mappingHandle;
	void * base;
};

struct ff_stream
{
	struct ff_db * db;
	DWORD firstBlock;
	DWORD length;
	DWORD cursor;
	DWORD curBlock;
	DWORD blockCursor;
};

void __inline _error( char const * e )
{
	fflush( 0 );
	fprintf( stderr, "libflashfiler: %s\n", e );
}

#define error( e, label ) \
	{ _error( e ); goto label; }

#pragma pack( push, 1 )
struct ff_header_common
{
	DWORD signature;
	DWORD nextBlock;
	DWORD thisBlock;
	DWORD lsn;
};

struct ff_header_file
{
	struct ff_header_common h;
	DWORD blockSize;
	DWORD encrypted;
	DWORD logBlockSize;
	DWORD usedBlocks;
	DWORD availBlocks;
	DWORD firstFreeBlock;
	DWORD numRecords;
	DWORD numDeletedRecords;
	unsigned __int64 firstDeletedRecord;
	DWORD recordLength;
	DWORD recordLengthEx;
	DWORD recordsPerBlock;
	DWORD firstDataBlock;
	DWORD lastDataBlock;
	DWORD blobCount;
	unsigned __int64 deletedBlobHead;
	unsigned __int64 deletedBlobTail;
	DWORD autoIncValue;
	DWORD numIndexes;
	DWORD hasSequentialIndex;
	DWORD indexHeader;
	DWORD numFields;
	DWORD dataDict;
	DWORD ffVersion;
	DWORD reserved[5];
	unsigned char reserved2[892];
};

struct ff_header_data_block
{
	struct ff_header_common h;
	DWORD numRecords;
	DWORD recordLength;
	DWORD nextBlock;
	DWORD prevBlock;
};

struct ff_header_stream_block
{
	struct ff_header_common h;
	DWORD nextBlock;
	DWORD streamType;
	DWORD streamLength;
	DWORD owningStream;
};

struct ff_dictent
{
	char name[64];
	int type;
	int length;
	int offset;
	DWORD streamOffset;
	int hasConstraints;
};

struct ff_dict
{
	struct ff_db * db;
	int numFields;
	struct ff_dictent * fields;
};

void * ff_getblockptr( struct ff_db * db, DWORD block );	// in ff_getblockptr.c
struct ff_stream * ff_openstream( struct ff_db * db, DWORD stream );
void ff_closestream( struct ff_stream * s );
int ff_readstream( struct ff_stream * s, void * buf, int len );
void ff_seekstream( struct ff_stream * s, int d );
struct ff_dict * ff_opendict( struct ff_db * db );

char ff_stream_readbyte( struct ff_stream * s );
int ff_stream_readbool( struct ff_stream * s );
int ff_stream_readint( struct ff_stream * s );
int ff_stream_readstring( struct ff_stream * s, char * buf );
char * ff_stream_readbin( struct ff_stream * s, int n );
void ff_stream_skipstring( struct ff_stream * s );
void ff_stream_skipbin( struct ff_stream * s, int n );
#define ff_stream_skipint ff_stream_readint
#define ff_stream_skipbool ff_stream_readbool

#pragma pack( pop )

#endif