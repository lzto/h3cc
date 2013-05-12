/*
    Copyright (C) 2010 lovewilliam<lovewiliam@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef MD5_CTX_H
#define MD5_CTX_H


/*
*NOTE : From OpenSSL
*/

class MD5_CTX
{
    public:
	MD5_CTX();
	virtual ~MD5_CTX();
	void MD5Update ( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);
	
    private:
	unsigned long int state[4];		/* state (ABCD) */
	unsigned long int count[2];		/* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];       /* input buffer */
	unsigned char PADDING[64];		/* What? */
	
    private:
	void MD5Init ();
	void MD5Transform (unsigned long int state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
	void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);
	
};

#endif // MD5_CTX_H
