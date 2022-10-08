//============================================================================
// Copyright (C) 2022 Brett R. Jones
//
// You may use, copy, modify, merge, publish, distribute, sub-license, and/or sell this software
// provided this Copyright is not modified or removed and is included all copies or substantial portions of the Software
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// bjones.engineer@gmail.com
// http://www.nolimitconnect.org
//============================================================================

#include "Sha1GeneratorMgr.h"
#include "VxFileUtil.h"
#include "VxGlobals.h"

namespace
{
	//============================================================================
    static void * Sha1GeneratorThreadFunc( void * pvContext )
	{
		VxThread* vxThread = (VxThread*)pvContext;
		vxThread->setIsThreadRunning( true );
		Sha1GeneratorMgr * sha1Generator = (Sha1GeneratorMgr *)vxThread->getThreadUserParam();
		sha1Generator->threadGenerateSha1( vxThread );
		vxThread->threadAboutToExit();
        return nullptr;
	}

	Sha1GeneratorMgr * g_Sha1GeneratorMgr = 0;
}

//============================================================================
Sha1GeneratorMgr& GetSha1GeneratorMgr( void )
{
	if( 0 == g_Sha1GeneratorMgr )
	{
		g_Sha1GeneratorMgr = new Sha1GeneratorMgr();
	}

	return *g_Sha1GeneratorMgr;
}

//============================================================================
Sha1GeneratorMgr::Sha1GeneratorMgr()
: m_Sha1ListMutex()
, m_Sha1Thread()
, m_Sha1List()
{
}

//============================================================================
void Sha1GeneratorMgr::generateSha1( VxGUID& fileId, std::string& fileName, Sha1GeneratorCallback* client )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	Sha1ClientInfo clienInfo( fileId, fileName, client );
	m_Sha1ListMutex.lock();

	for( auto iter = m_Sha1List.begin(); iter != m_Sha1List.end(); ++iter )
	{
		if( *iter == clienInfo )
		{
			// duplicate
			LogMsg( LOG_WARN, "Sha1GeneratorMgr::generateSha1 duplicate request file %s", clienInfo.getFileName().c_str() );
			m_Sha1ListMutex.unlock();
			announceResult( eSha1GenResultDuplicateRequest, clienInfo );
			return;
		}
	}

	m_Sha1List.push_back( clienInfo );
	startThreadIfNotStarted();
	m_Sha1ListMutex.unlock();	
}

//============================================================================
void Sha1GeneratorMgr::cancelGenerateSha1( VxGUID& fileId, std::string& fileName, Sha1GeneratorCallback* client )
{
	if( VxIsAppShuttingDown() )
	{
		return;
	}

	Sha1ClientInfo clienInfo( fileId, fileName, client );
	m_Sha1ListMutex.lock();
	for( auto iter = m_Sha1List.begin(); iter != m_Sha1List.end(); ++iter )
	{
		if( *iter == clienInfo )
		{
			LogMsg( LOG_VERBOSE, "Sha1GeneratorMgr::cancelGenerateSha1 file %s", clienInfo.getFileName().c_str() );
			m_Sha1List.erase( iter );
			break;
		}
	}

	m_Sha1ListMutex.unlock();
}

//============================================================================
void Sha1GeneratorMgr::startThreadIfNotStarted( void )
{
	if( false == m_Sha1Thread.isThreadRunning() )
	{
		m_Sha1Thread.startThread( (VX_THREAD_FUNCTION_T)Sha1GeneratorThreadFunc, this, "Sha1GeneratorThread" );
	}
}

#define SHRED_BUF_SIZE 1024 * 16
//============================================================================
void Sha1GeneratorMgr::threadGenerateSha1( VxThread* vxThread )
{
	while( false == m_Sha1Thread.isAborted() && !VxIsAppShuttingDown() )
	{
		std::string fileName ;
		if( 0 != m_Sha1List.size() )
		{
			m_Sha1ListMutex.lock();
			if( 0 != m_Sha1List.size() )
			{
				Sha1ClientInfo clientInfo = m_Sha1List[0];
				m_Sha1List.erase( m_Sha1List.begin() );
				m_Sha1ListMutex.unlock();

				if( clientInfo.isValid( false ) )
				{
					if( clientInfo.getSha1Info().getSha1Hash().generateHashFromFile( clientInfo.getFileName().c_str(), vxThread ) )
					{
						announceResult( eSha1GenResultNoError, clientInfo );
					}
					else
					{
						LogMsg( LOG_ERROR, "Sha1GeneratorMgr::threadGenerateSha1 failed gen sha1 %s", clientInfo.getFileName().c_str() );
						announceResult( eSha1GenResultGenerateSha1Failed, clientInfo );
					}
				}
				else
				{
					LogMsg( LOG_ERROR, "Sha1GeneratorMgr::threadGenerateSha1 Invalid Param" );
					announceResult( eSha1GenResultInvalidParam, clientInfo );
				}
			}
			else
			{
				m_Sha1ListMutex.unlock();
				LogMsg( LOG_VERBOSE, "Sha1GeneratorMgr::threadGenerateSha1 done" );
			}
		}

		m_Sha1ListMutex.lock();	
		if( 0 == m_Sha1List.size() )
		{
			m_Sha1ListMutex.unlock();	
			break;
		}
		else
		{
			m_Sha1ListMutex.unlock();	
		}
	}
}

//============================================================================
void Sha1GeneratorMgr::announceResult( ESha1GenResult sha1GenResult, Sha1ClientInfo& sha1Info )
{
	if( sha1Info.getClient() )
	{
		sha1Info.getClient()->callbackSha1GenerateResult( sha1GenResult, sha1Info.getSha1Info().getAssetId(), sha1Info.getSha1Info() );
	}
	else
	{
		LogMsg( LOG_ERROR, "Sha1GeneratorMgr::announceResult null client" );
	}
}