#ifdef LAUNCH_GDBSERVER_FROM_APP
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <android/log.h>
#include <sys/socket.h>
#include <netinet/in.h>

static const char AppName[] = "VisualGDB GDBServer Launcher";

struct ControlBlock
{
    int GDBServerPort;
    char GDBServerPath[1];
};

static void DoLaunchGDBServer(const struct ControlBlock *pBlock, int socket)
{
    char pid[32], port[32];
    sprintf(pid, "%d", getpid());
    sprintf(port, ":%d", pBlock->GDBServerPort);
    
    if (fork() == 0)
    {
        dup2(socket, STDOUT_FILENO);
        dup2(socket, STDERR_FILENO);

        int r = execl(pBlock->GDBServerPath,
            "gdbserver",
            port,
            "--attach",
            pid,
            NULL);

        //if execl() succeeds, control will never reach here
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to launch %s: error %d", pBlock->GDBServerPath, r);
    }
}

void LaunchGDBServer()
{
	struct sockaddr_in addr = { AF_INET, };
	int socket1 = socket(AF_INET, SOCK_STREAM, 0);
    int32_t controlBlockLen = 0;
    int socket2 = 0;
    socklen_t len = sizeof(addr);
    int r, mypid;
    struct ControlBlock *pBlock = NULL;
    
    if (socket1 < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to open a socket. Ensure your app contains debug permissions.");
        goto cleanup;
    }
    
    r = bind(socket1, (struct sockaddr *)&addr, sizeof(addr));
    if (r)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to bind the command socket. Ensure your app contains debug permissions.");
        goto cleanup;
    }
    
    r = listen(socket1, 1);
    if (r)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to bind the command socket. Ensure your app contains debug permissions.");
        goto cleanup;
    }
    
    r = getsockname(socket1, (struct sockaddr*)&addr, &len);
    if (r)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to get local socket port. Ensure your app contains debug permissions.");
        goto cleanup;
    }

    mypid = getpid();
    if (fork() == 0)
    {
        __android_log_print(ANDROID_LOG_INFO, AppName, "GDBServer launcher for PID %d: listening on port %d.", mypid, ntohs(addr.sin_port));
        exit(0);        
    }
    
    len = sizeof(addr);
    socket2 = accept(socket1, (struct sockaddr *)&addr, &len);
    
    if (socket2 < 0)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to accept incoming connection on the command socket. Ensure your app contains debug permissions.");
        goto cleanup;
    }
    
    r = recv(socket2, &controlBlockLen, sizeof(controlBlockLen), MSG_WAITALL);
    if (r != sizeof(controlBlockLen))
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to receive control block length: recv() returned %d", r);
        goto cleanup;
    }
    
    pBlock = (struct ControlBlock *)malloc(controlBlockLen + 1);
    if (!pBlock)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to allocate a control block.");
        goto cleanup;
    }
    
	r = recv(socket2, pBlock, controlBlockLen, MSG_WAITALL);
    if (r != controlBlockLen)
    {
        __android_log_print(ANDROID_LOG_ERROR, AppName, "Failed to receive control block: received %d, expected %d.", r, controlBlockLen);
        goto cleanup;
    }
    
    ((char *)pBlock)[controlBlockLen] = 0;
    DoLaunchGDBServer(pBlock, socket2);

cleanup:
    if (pBlock)
        free(pBlock);
    if (socket1 > 0)
        close(socket1);
}

__attribute__((constructor)) void GDBServerInitializer()
{
    LaunchGDBServer();
}

#endif