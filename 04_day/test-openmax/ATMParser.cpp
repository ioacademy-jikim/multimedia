/*
**
** Copyright 2013, kod21236@gmail.com
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#define LOG_TAG "ATMParser"

#include "ATMParser.h"
#include <ctype.h>
#include <string.h>
#include <utils/Log.h>

#define LOGD ALOGD



namespace anatomy {

static char pszSource[] = "\
M9a1X1M13B1M55/1M9B1Z1a1M11B1W1a1M54/1M8Z1r1X2M10@1r1S1;1Z1M53/1M9Z1X1r1M3\
W2B1W2M3r1X2M6r1X1M12Z1i1X1M4X1;1X1M24/1M10B1X1S2X7S2X1W1M7.1 1Z1M11X1 2X1M3:1 2\
M6@1M17/1M9B1S2X2a1S1a1S1a1S1a1S3a1W1M7 1r1:1i1M3S1 2M5 1X1M5 1M4:1 2,1M16/1M8X5\
S1X1S2a1S2X6a1M6 2S1 1S1M1B1 1W1,1 1M4 1S1M5 1M3Z1 1M2 1S1M15/1M6@1X3Z1B5S2a1B5a\
1X2S1M5 1S1M1 1X1M1:1 1;1i1 1M4 1S1M5 1M3i1 1M2.1,1M15/1M6X1S2X1@1M5a1X1B1M5B1S2\
X1a1M4 1W1M1 1Z1M1X1 1M2i1M4 1Z1M5 1M3a1 1M2 1X1M1Z1 1a1M11/1M5X2S1a1S1Z6S2a1Z1B\
1Z4S1a1S1X1B1M2.1 2Z1 2W1M1 3;1M2X1 4M2 5M2.1 3M2B1 2M11/1M4W1X1S1a1S1a1S2X3S2a1\
S2X3S2a3S1X1M2B1Z1B1M1B2M3B1@1M3@1Z1@2W1M2W1Z1@1W1@1M3W1B1M3a1 1W1M11/1M2@1B1S2a\
1S1a1S1a1S3a1S1a1S1a1S1a1S1a1S1a1S1a1S1a1X1Z1M35B1Z1M12/1M1B1r1X2a1S1X20S2a1r1X2\
Z1M46/1Z1S2a1S2Z1M6@1M5@1M1@1M5@1S2X1a1S1r1B1M45/1X1Z1S1Z1X1S1Z1M21S1a1X1a1S1X2M\
45/1B1S1a2S2a1X19S1X1a2S1a2X1a1M45/1M1B1r1S1X1a2S3X1S1X1S9X1S6a1X1a1X2@1M45/1M2B\
1X3a4Z1a18S1r1X1Z1M47/1M4W1X1S1a8Z1a12S1X1Z1M23a1X1M24/1M5S1X1S1a6Z1a13S1X1M4B1W\
1M11W1B1M5 2M5Z1X1M17/1M5@1X1a1S1a8Z1a7Z1a2X1Z1M2X1 3X1M2 2,1 2M2 4M2r1 4:1M2 4M\
2 6W2 2S1i1 1.1/1M6Z1X1a10Z1a8X2M3a1:1M1 2M2@1 1i1M1 1M2.1W1r1 1M4 1:1M3B1 1M2 1\
i1M1i1 1M1 1M1 1X1M1X1 1M2 1M1/1M7a1X1a2Z1a13Z1S1X1W1M3B1 2S1X1M3 1M2 1M2i1 1;1i\
1M4.1r1M1@1M1X1 1M2i1 1M1X1,1M1 1M1,1r1M2 1S2i1M1/1M8Z1X1a5S1a1S1a6Z1S1X1W1M4 2Z\
2 2M2 1Z1M1 1W1B1 2M1 2r1M2 2a1 1M1@1 1W1M1 1S1M1r1 1M1 1M1 1i1M3 2M2/1M9X1r1S1a\
1Z1S3a1S1a1S1a3X3@1M4X1 2i1 1.1M1.1 2r1 2M1 2.1 2W1M2r1 2a1M2X1 2i1M2 2,1 1,1 2M\
3.1 1M2/1M9X1S1X3S1X9S1a1X1Z1M46B1 2Z1M2/1M9X1S1Z1S1X1M3@3M2X1a2S1r1B1M46W1 1.1M\
3/1M9@1a1Z3M8W1a1Z1a1Z1M53";

ATMParser::ATMParser()
    : mOffset(0),
    mLength(sizeof(pszSource)) {

}

void ATMParser::read(void* buffer, int* length) {
	int i = 0;

	if ( mOffset < mLength )
	{
		for ( i = mOffset+1; i < mLength; i++ )
		{
			if ( !isdigit(pszSource[i]) )
				break;
		}
		*length = i - mOffset + 1;
		memcpy(buffer, &pszSource[mOffset], *length);
		mOffset = i;
		LOGD("mOffset : %d , mLength : %d, *length : %d", mOffset, mLength, *length);
	}
};

}

