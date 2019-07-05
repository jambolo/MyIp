/*********************************************************************************************************************

                                                       MyIp.cpp

						                    Copyright 2002, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/MyIp/MyIp.cpp#3 $

	$NoKeywords: $

*********************************************************************************************************************/

#include "stdafx.h"

static char const	s_LocalHost[]	= "127.0.0.1";

/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

int main(int argc, char* argv[])
{
	enum OutputFormat
	{
		OF_NUMBER,
		OF_NAME,
		OF_FULLNAME,
	};

	OutputFormat	outputFormat	= OF_NUMBER;	// Output format (default is number)

	// Get command line options

	--argc;
	++argv;

	if ( argc > 0 )
	{
		if ( strcmp( *argv, "-name" ) == 0 )
		{
			outputFormat = OF_NAME;
		}
		else if ( strcmp( *argv, "-fullname" ) == 0 )
		{
			outputFormat = OF_FULLNAME;
		}
	}

	int		rv;

	// Start up winsock

	WSADATA wsaData;
 
	rv = WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
	if ( rv != 0 )
	{
		std::cerr << "WSAStartup failed, error code = " << rv << std::endl;
		return 1;
	}

	// Get our host name

	char	hostname[ 256 ];						// This host's name
	int		exitCode			= 0;				// Exit code (defaults to 0)

	rv = gethostname( hostname, sizeof hostname );
	if ( rv != 0 )
	{
		std::cerr << "gethostname failed, error code = " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// If the host name was desired, then just output it and we are done

	if ( outputFormat == OF_NAME )
	{
		std::cout << hostname;
		WSACleanup();
		return 0;
	}

	// Get our IP address and full name from our host name

	HOSTENT	const * const hostInfo = gethostbyname( hostname );

	if ( hostInfo == NULL )
	{
		std::cerr << "gethostbyname( \"" << hostname << "\" ) failed, error code = " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// Output the results

	if ( outputFormat == OF_FULLNAME )
	{
		std::cout << hostInfo->h_name;
	}
	else
	{
		if ( hostInfo->h_length <= 0 || hostInfo->h_addr_list[0] == NULL )
		{
			std::cerr << "gethostbyname( \"" << hostname << "\" ) returned a blank IP address" << std::endl;
			std::cout << s_LocalHost << std::endl;
		}


		std::cout << unsigned( unsigned char( hostInfo->h_addr_list[0][0] ) );
		for ( int i = 1; i < hostInfo->h_length; i++ )
		{
			std::cout << '.' << unsigned( unsigned char( hostInfo->h_addr_list[0][i] ) );
		}
	}



	// Shut down winsock

	WSACleanup();

	// Return success

	return 0;
}
