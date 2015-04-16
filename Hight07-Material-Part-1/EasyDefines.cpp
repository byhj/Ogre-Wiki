//
// Copyright (c) 2007-2011, 'Madmarx' from the ogre3D forums (ogre3D.org).
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of its author nor the names of its contributors
//       may be used to endorse or promote products derived from this software
//       without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ------------------------------------------------------------------------------//
// ------------------------------------------------------------------------------//

#include "EasyDefines.h"

#include "CImg.h"
namespace OgreEasy
{
	void showMessageBox(std::string pText)
	{
		typedef std::string TString;
		unsigned int maxcharwidth = 60;
		unsigned int nblines = 1;
		for(TString::size_type i = 0; i < pText.size(); i += (maxcharwidth+1))
		{
			pText.insert(i,"\n");
		}
		for(TString::size_type i = 0; i < pText.size(); ++i)
		{
			if('\n' == pText[i])
			{
				++nblines;
			}
		}
		cimg_library::CImg<unsigned char> lImage(maxcharwidth * 10, nblines * 11,1,1);
		unsigned char black = 0;
		unsigned char white = 255;
		lImage.draw_text(0,0, pText.c_str(), &black, &white, 1.0f, 10);
		cimg_library::CImgDisplay display(lImage);
		while(!display.is_closed)
		{
			display.wait();

			if(display.is_keyENTER || display.is_keyESC)
			{
				display.close();
			}
		}
	}
	void waitForUser()
	{
		showMessageBox("Press enter");
	}
}
