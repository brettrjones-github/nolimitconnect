/*
   minizip.c
   Version 1.1, February 14h, 2010
   sample part of the MiniZip project - ( http://www.winimage.com/zLibDll/minizip.html )

         Copyright (C) 1998-2010 Gilles Vollant (minizip) ( http://www.winimage.com/zLibDll/minizip.html )

         Modifications of Unzip for Zip64
         Copyright (C) 2007-2008 Even Rouault

         Modifications for Zip64 support on both zip and unzip
         Copyright (C) 2009-2010 Mathias Svensson ( http://result42.com )
*/

#include <NlcDependLibrariesConfig.h>


#if (!defined(_WIN32)) && (!defined(WIN32)) && (!defined(__APPLE__)) && (!defined(IOAPI_NO_64))
        #ifndef __USE_FILE_OFFSET64
                #define __USE_FILE_OFFSET64
        #endif
        #ifndef __USE_LARGEFILE64
                #define __USE_LARGEFILE64
        #endif
        #ifndef _LARGEFILE64_SOURCE
                #define _LARGEFILE64_SOURCE
        #endif
        #ifndef _FILE_OFFSET_BIT
                #define _FILE_OFFSET_BIT 64
        #endif
#endif

#if defined( __APPLE__ ) || defined(IOAPI_NO_64)
// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#define FTELLO_FUNC(stream) ftello(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#define FTELLO_FUNC(stream) ftello64(stream)
#define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef _WIN32
# include <direct.h>
# include <io.h>
#else
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#endif

#include "MiniZip.h"
#include <CoreLib/VxDebug.h>

#ifdef _WIN32
        #define USEWIN32IOAPI
        #include "iowin32.h"
#endif

#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

#ifdef _WIN32
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
  int ret = 0;
  {
      FILETIME ftLocal;
      HANDLE hFind;
      WIN32_FIND_DATAA ff32;

      hFind = FindFirstFileA(f,&ff32);
      if (hFind != INVALID_HANDLE_VALUE)
      {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
        FindClose(hFind);
        ret = 1;
      }
  }
  return ret;
}
#else
uLong filetime(f, tmzip, dt)
    char *f;               /* name of file to get info on */
    tm_zip *tmzip;         /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
  int ret=0;
  struct stat s;        /* results of stat() */
  struct tm* filedate;
  time_t tm_t=0;

  if (strcmp(f,"-")!=0)
  {
    char name[MAXFILENAME+1];
    int len = strlen(f);
    if (len > MAXFILENAME)
      len = MAXFILENAME;

    strncpy(name, f,MAXFILENAME-1);
    /* strncpy doesnt append the trailing NULL, of the string is too long. */
    name[ MAXFILENAME ] = '\0';

    if (name[len - 1] == '/')
      name[len - 1] = '\0';
    /* not all systems allow stat'ing a file with / appended */
    if (stat(name,&s)==0)
    {
      tm_t = s.st_mtime;
      ret = 1;
    }
  }
  filedate = localtime(&tm_t);

  tmzip->tm_sec  = filedate->tm_sec;
  tmzip->tm_min  = filedate->tm_min;
  tmzip->tm_hour = filedate->tm_hour;
  tmzip->tm_mday = filedate->tm_mday;
  tmzip->tm_mon  = filedate->tm_mon ;
  tmzip->tm_year = filedate->tm_year;

  return ret;
}
#endif




int check_exist_file(filename)
    const char* filename;
{
    FILE* ftestexist;
    int ret = 1;
    ftestexist = FOPEN_FUNC(filename,"rb");
    if (ftestexist==NULL)
        ret = 0;
    else
        fclose(ftestexist);
    return ret;
}

void do_minizip_banner()
{
    printf("MiniZip 1.1, demo of zLib + MiniZip64 package, written by Gilles Vollant\n");
    printf("more info on MiniZip at http://www.winimage.com/zLibDll/minizip.html\n\n");
}

void do_minizip_help()
{
    printf("Usage : minizip [-o] [-a] [-0 to -9] [-p password] [-j] file.zip [files_to_add]\n\n" \
           "  -o  Overwrite existing file.zip\n" \
           "  -a  Append to existing file.zip\n" \
           "  -0  Store only\n" \
           "  -1  Compress faster\n" \
           "  -9  Compress better\n\n" \
           "  -j  exclude path. store only the file name.\n\n");
}

/* calculate the CRC32 of a file,
   because to encrypt a file, we need known the CRC32 of the file before */
int getFileCrc(const char* filenameinzip,void*buf,unsigned long size_buf,unsigned long* result_crc)
{
   unsigned long calculate_crc=0;
   int err=ZIP_OK;
   FILE * fin = FOPEN_FUNC(filenameinzip,"rb");

   unsigned long size_read = 0;
   unsigned long total_read = 0;
   if (fin==NULL)
   {
       err = ZIP_ERRNO;
   }

    if (err == ZIP_OK)
        do
        {
            err = ZIP_OK;
            size_read = (int)fread(buf,1,size_buf,fin);
            if (size_read < size_buf)
                if (feof(fin)==0)
            {
                printf("error in reading %s\n",filenameinzip);
                err = ZIP_ERRNO;
            }

            if (size_read>0)
                calculate_crc = crc32(calculate_crc,buf,size_read);
            total_read += size_read;

        } while ((err == ZIP_OK) && (size_read>0));

    if (fin)
        fclose(fin);

    *result_crc=calculate_crc;
#ifdef DEBUG
    printf("MiniZip file %s crc %lx\n", filenameinzip, calculate_crc);
#endif
    return err;
}

int isLargeFile(const char* filename)
{
  int largeFile = 0;
  ZPOS64_T pos = 0;
  FILE* pFile = FOPEN_FUNC(filename, "rb");

  if(pFile != NULL)
  {
    int n = FSEEKO_FUNC(pFile, 0, SEEK_END);
    pos = FTELLO_FUNC(pFile);

#ifdef DEBUG
    printf("MiniZip isLargeFile File : %s is %lld bytes\n", filename, pos);
#endif

    if(pos >= 0xffffffff)
     largeFile = 1;

                fclose(pFile);
  }

 return largeFile;
}


typedef struct zip_run_options_s
{
	int				opt_overwrite;
	int				opt_compress_level;
	int				opt_exclude_path;
	int				zipfilenamearg;
	char			filename_try[MAXFILENAME+16];
	const char*		password;
	int				zipfileCount;
	////
	const char *	filenameinzip;

} zip_run_options_s;

zip_run_options_s g_ZipRunOptions;

void Zip_ResetOptions( void )
{
	g_ZipRunOptions.opt_overwrite		= 1;
	g_ZipRunOptions.opt_compress_level	= Z_DEFAULT_COMPRESSION;
	g_ZipRunOptions.opt_exclude_path	= 0;
	g_ZipRunOptions.zipfilenamearg		= 0;
	g_ZipRunOptions.filename_try[0]		= 0;
	g_ZipRunOptions.password			= NULL;
	g_ZipRunOptions.zipfileCount		= 0;

	g_ZipRunOptions.filenameinzip		= NULL;
}

void Zip_SetCompressionRate( int rate0to9_0faster_9better )
{
	g_ZipRunOptions.opt_compress_level	= rate0to9_0faster_9better;
}

void Zip_SetFileToZipInto( const char * fileName )
{
#ifdef WIN32
	// flip forward slash to back slash the way zip lib likes it
	unsigned int i;
	static char fileNameCopy[ 256 ];
	strcpy( fileNameCopy, fileName );
	for( i = 0; i < strlen( fileName ); ++i )
	{
		if( fileNameCopy[i] == '/' )
		{
			fileNameCopy[i] = '\\';
		}
	}
	strcpy( g_ZipRunOptions.filename_try, fileNameCopy );
#else
	strcpy( g_ZipRunOptions.filename_try, fileName );
#endif // WIN32
}

void Zip_SetFileCount( int fileCount )
{
	g_ZipRunOptions.zipfileCount = fileCount;
}

void Zip_SetFileNameInZip( const char * fileName )
{
	g_ZipRunOptions.filenameinzip = fileName;
}


int Zip_ExecuteZip( void * zipper )
{
	int size_buf=0;
	void* buf=NULL;
	int i;

	int err=0;

	size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        LogMsg( LOG_ERROR, "Error allocating memory\n");
        return ZIP_INTERNALERROR;
    }
	else
	{

		zipFile zf;
		int errclose;
# ifdef USEWIN32IOAPI
		zlib_filefunc64_def ffunc;
		fill_win32_filefunc64A(&ffunc);
		zf = zipOpen2_64(g_ZipRunOptions.filename_try,(g_ZipRunOptions.opt_overwrite==2) ? 2 : 0,NULL,&ffunc);
# else
		zf = zipOpen64( g_ZipRunOptions.filename_try,(g_ZipRunOptions.opt_overwrite==2) ? 2 : 0);
# endif // USEWIN32IOAPI

		if (zf == NULL)
		{
			LogMsg( LOG_ERROR, "error opening %s\n", g_ZipRunOptions.filename_try );
			err= ZIP_ERRNO;
		}
		else
		{
#ifdef DEBUG
			LogMsg( LOG_ERROR, "creating %s\n", g_ZipRunOptions.filename_try );
#endif
		}

		for( i = 0; i < g_ZipRunOptions.zipfileCount; i++ )
		{
			Zip_GetNextFileName( zipper );
			if( NULL != g_ZipRunOptions.filenameinzip )
			{
				FILE * fin;
				int size_read;
				const char *savefilenameinzip;
				zip_fileinfo zi;
				unsigned long crcFile=0;
				int zip64 = 0;

				zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
				zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
				zi.dosDate = 0;
				zi.internal_fa = 0;
				zi.external_fa = 0;
				filetime(g_ZipRunOptions.filenameinzip,&zi.tmz_date,&zi.dosDate);

	/*
				err = zipOpenNewFileInZip(zf,filenameinzip,&zi,
								 NULL,0,NULL,0,NULL / * comment * /,
								 (opt_compress_level != 0) ? Z_DEFLATED : 0,
								 opt_compress_level);
	*/
				if ((g_ZipRunOptions.password != NULL) && (err==ZIP_OK))
					err = getFileCrc(g_ZipRunOptions.filenameinzip,buf,size_buf,&crcFile);

				zip64 = isLargeFile(g_ZipRunOptions.filenameinzip);

														 /* The path name saved, should not include a leading slash. */
			   /*if it did, windows/xp and dynazip couldn't read the zip file. */
				 savefilenameinzip = g_ZipRunOptions.filenameinzip;
				 while( savefilenameinzip[0] == '\\' || savefilenameinzip[0] == '/' )
				 {
					 savefilenameinzip++;
				 }

				 /*should the zip file contain any path at all?*/
				 if( g_ZipRunOptions.opt_exclude_path )
				 {
					 const char *tmpptr;
					 const char *lastslash = 0;
					 for( tmpptr = savefilenameinzip; *tmpptr; tmpptr++)
					 {
						 if( *tmpptr == '\\' || *tmpptr == '/')
						 {
							 lastslash = tmpptr;
						 }
					 }
					 if( lastslash != NULL )
					 {
						 savefilenameinzip = lastslash+1; // base filename follows last slash.
					 }
				 }

				 /**/
				err = zipOpenNewFileInZip3_64(zf,savefilenameinzip,&zi,
								 NULL,0,NULL,0,NULL /* comment*/,
								 (g_ZipRunOptions.opt_compress_level != 0) ? Z_DEFLATED : 0,
								 g_ZipRunOptions.opt_compress_level,0,
								 /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
								 -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
								 g_ZipRunOptions.password,crcFile, zip64);

				if (err != ZIP_OK)
					LogMsg( LOG_ERROR, "error in opening %s in zipfile\n",g_ZipRunOptions.filenameinzip);
				else
				{
					fin = FOPEN_FUNC(g_ZipRunOptions.filenameinzip,"rb");
					if (fin==NULL)
					{
						err=ZIP_ERRNO;
						LogMsg( LOG_ERROR, "error in opening %s for reading\n",g_ZipRunOptions.filenameinzip);
					}
				}

				if (err == ZIP_OK)
					do
					{
						err = ZIP_OK;
						size_read = (int)fread(buf,1,size_buf,fin);
						if (size_read < size_buf)
							if (feof(fin)==0)
						{
							printf("error in reading %s\n",g_ZipRunOptions.filenameinzip);
							err = ZIP_ERRNO;
						}

						if (size_read>0)
						{
							err = zipWriteInFileInZip (zf,buf,size_read);
							if (err<0)
							{
								printf("error in writing %s in the zipfile\n",
												 g_ZipRunOptions.filenameinzip);
							}

						}
					} while ((err == ZIP_OK) && (size_read>0));

				if (fin)
					fclose(fin);

				if (err<0)
					err=ZIP_ERRNO;
				else
				{
					err = zipCloseFileInZip(zf);
					if (err!=ZIP_OK)
						printf("error in closing %s in the zipfile\n",
									g_ZipRunOptions.filenameinzip);
				}
			}
		}
		errclose = zipClose(zf,NULL);
		if (errclose != ZIP_OK)
			printf("error in closing %s\n",g_ZipRunOptions.filename_try);

		free(buf);
		return 0;
	}
	}

