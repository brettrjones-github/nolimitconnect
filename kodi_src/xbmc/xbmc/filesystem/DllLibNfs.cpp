
#include "DllLibNfs.h"

#if USE_STATIC_LIBS

DllLibNfs::DllLibNfs()
    : StaticDll( DLL_PATH_LIBNFS )
{
}

void DllLibNfs::mount_free_export_list( struct exportnode *exports )
{
    ::mount_free_export_list( exports );
}

struct exportnode * DllLibNfs::mount_getexports( const char *server )
{
    return ::mount_getexports( server );
}

struct nfs_server_list * DllLibNfs::nfs_find_local_servers( void )
{
    return ::nfs_find_local_servers( );
}

void  DllLibNfs::free_nfs_srvr_list( struct nfs_server_list *srv )
{
    ::free_nfs_srvr_list( srv );
}

struct nfs_context *  DllLibNfs::nfs_init_context( void )
{
    return ::nfs_init_context( );
}

void  DllLibNfs::nfs_destroy_context( struct nfs_context *nfs )
{
    return ::nfs_destroy_context( nfs );
}

uint64_t DllLibNfs::nfs_get_readmax( struct nfs_context *nfs )
{
    return ::nfs_get_readmax( nfs );
}

uint64_t DllLibNfs::nfs_get_writemax( struct nfs_context *nfs )
{
    return ::nfs_get_writemax( nfs );
}

char * DllLibNfs::nfs_get_error( struct nfs_context *nfs )
{
    return ::nfs_get_error( nfs );
}

int DllLibNfs::nfs_close( struct nfs_context *nfs, struct nfsfh *nfsfh )
{
    return ::nfs_close( nfs, nfsfh );
}

int DllLibNfs::nfs_fsync( struct nfs_context *nfs, struct nfsfh *nfsfh )
{
    return ::nfs_fsync( nfs, nfsfh );
}

int DllLibNfs::nfs_mkdir( struct nfs_context *nfs, const char *path )
{
    return ::nfs_mkdir( nfs, path );
}

int DllLibNfs::nfs_rmdir( struct nfs_context *nfs, const char *path )
{
    return ::nfs_rmdir( nfs, path );
}

int DllLibNfs::nfs_unlink( struct nfs_context *nfs, const char *path )
{
    return ::nfs_unlink( nfs, path );
}

void   DllLibNfs::nfs_closedir( struct nfs_context *nfs, struct nfsdir *nfsdir )
{
    ::nfs_closedir( nfs, nfsdir );
}

struct nfsdirent * DllLibNfs::nfs_readdir( struct nfs_context *nfs, struct nfsdir *nfsdir )
{
    return ::nfs_readdir( nfs, nfsdir );
}

int DllLibNfs::nfs_mount( struct nfs_context *nfs, const char *server, const char *exportname )
{
    return ::nfs_mount( nfs, server, exportname );
}

int DllLibNfs::nfs_stat( struct nfs_context *nfs, const char *path, NFSSTAT *st )
{
    return ::nfs_stat( nfs, path, st );
}

int DllLibNfs::nfs_fstat( struct nfs_context *nfs, struct nfsfh *nfsfh, NFSSTAT *st )
{
    return ::nfs_fstat( nfs, nfsfh, st );
}

int DllLibNfs::nfs_truncate( struct nfs_context *nfs, const char *path, uint64_t length )
{
    return ::nfs_truncate( nfs, path, length );
}

int DllLibNfs::nfs_ftruncate( struct nfs_context *nfs, struct nfsfh *nfsfh, uint64_t length )
{
    return ::nfs_ftruncate( nfs, nfsfh, length );
}

int DllLibNfs::nfs_opendir( struct nfs_context *nfs, const char *path, struct nfsdir **nfsdir )
{
    return ::nfs_opendir( nfs, path, nfsdir );
}

int DllLibNfs::nfs_statvfs( struct nfs_context *nfs, const char *path, struct statvfs *svfs )
{
    return ::nfs_statvfs( nfs, path, svfs );
}

int DllLibNfs::nfs_chmod( struct nfs_context *nfs, const char *path, int mode )
{
    return ::nfs_chmod( nfs, path, mode );
}

int DllLibNfs::nfs_fchmod( struct nfs_context *nfs, struct nfsfh *nfsfh, int mode )
{
    return ::nfs_fchmod( nfs, nfsfh, mode );
}

int DllLibNfs::nfs_access( struct nfs_context *nfs, const char *path, int mode )
{
    return ::nfs_access( nfs, path, mode );
}

int DllLibNfs::nfs_utimes( struct nfs_context *nfs, const char *path, struct timeval *times )
{
    return ::nfs_utimes( nfs, path, times );
}

int DllLibNfs::nfs_utime( struct nfs_context *nfs, const char *path, struct utimbuf *times )
{
    return ::nfs_utime( nfs, path, times );
}

int DllLibNfs::nfs_symlink( struct nfs_context *nfs, const char *oldpath, const char *newpath )
{
    return ::nfs_symlink( nfs, oldpath, newpath );
}

int DllLibNfs::nfs_rename( struct nfs_context *nfs, const char *oldpath, const char *newpath )
{
    return ::nfs_rename( nfs, oldpath, newpath );
}

int DllLibNfs::nfs_link( struct nfs_context *nfs, const char *oldpath, const char *newpath )
{
    return ::nfs_link( nfs, oldpath, newpath );
}

int DllLibNfs::nfs_readlink( struct nfs_context *nfs, const char *path, char *buf, int bufsize )
{
    return ::nfs_readlink( nfs, path, buf, bufsize );
}

int DllLibNfs::nfs_chown( struct nfs_context *nfs, const char *path, int uid, int gid )
{
    return ::nfs_chown( nfs, path, uid, gid );
}

int DllLibNfs::nfs_fchown( struct nfs_context *nfs, struct nfsfh *nfsfh, int uid, int gid )
{
    return ::nfs_fchown( nfs, nfsfh, uid, gid );
}

int DllLibNfs::nfs_open( struct nfs_context *nfs, const char *path, int mode, struct nfsfh **nfsfh )
{
    return ::nfs_open( nfs, path, mode, nfsfh );
}

int DllLibNfs::nfs_read( struct nfs_context *nfs, struct nfsfh *nfsfh, uint64_t count, char *buf )
{
    return ::nfs_read( nfs, nfsfh, count, buf );
}

int DllLibNfs::nfs_write( struct nfs_context *nfs, struct nfsfh *nfsfh, uint64_t count, char *buf )
{
    return ::nfs_write( nfs, nfsfh, count, buf );
}

int DllLibNfs::nfs_creat( struct nfs_context *nfs, const char *path, int mode, struct nfsfh **nfsfh )
{
    return ::nfs_creat( nfs, path, mode, nfsfh );
}

int DllLibNfs::nfs_pread( struct nfs_context *nfs, struct nfsfh *nfsfh, uint64_t offset, uint64_t count, char *buf )
{
    return ::nfs_pread( nfs, nfsfh, offset, count, buf );
}

int DllLibNfs::nfs_pwrite( struct nfs_context *nfs, struct nfsfh *nfsfh, uint64_t offset, uint64_t count, char *buf )
{
    return ::nfs_pwrite( nfs, nfsfh, offset, count, buf );
}

int DllLibNfs::nfs_lseek( struct nfs_context *nfs, struct nfsfh *nfsfh, uint64_t offset, int whence, uint64_t *current_offset )
{
    return ::nfs_lseek( nfs, nfsfh, offset, whence, current_offset );
}



#endif // USE_STATIC_LIBS
