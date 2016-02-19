/////////////////////////////////////////////////////////////////////////////
// VRaptorPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "VRaptorPlugIn.h"



// The plug-in object must be constructed before any plug-in classes
// derived from CRhinoCommand. The #pragma init_seg(lib) ensures that
// this happens.

#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME( L"VRaptor" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"295D51BF-1B63-455A-9331-CD3E593494AB" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
// TODO: fill in the following developer declarations with
// your company information. Note, all of these declarations
// must be present or your plug-in will not load.
//
// When completed, delete the following #error directive.
// #error Developer declarations block is incomplete!
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"ekswhyzee" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"Cambridge ON" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"Canada" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"226-338-6626" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"NA" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"jakerobertread@gmail.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.ekswhyzee.com" );
RHINO_PLUG_IN_UPDATE_URL( L"http://www.ekswhyzee.com" );

// The one and only CVRaptorPlugIn object
static CVRaptorPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CVRaptorPlugIn definition

CVRaptorPlugIn& VRaptorPlugIn()
{ 
  // Return a reference to the one and only CVRaptorPlugIn object
  return thePlugIn; 
}


CVRaptorPlugIn::CVRaptorPlugIn()
{
  // Description:
  //   CVRaptorPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CVRaptorPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CVRaptorPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CVRaptorPlugIn::~CVRaptorPlugIn()
{
  // Description:
  //   CVRaptorPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CVRaptorPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// US ///

// shorthand 4 us
CVRaptorPlugIn& VR()
{
	return thePlugIn;
}

void ovrWinWomb()
{
		// BEGIN what was WINAPI WinMain
	OVR::System::Init(); // we're probably going to have to de-construct this page & rebuild in the plugin. best of luck.

    // Initializes LibOVR, and the Rift
    ovrResult result001 = ovr_Initialize(nullptr);
	if (!OVR_SUCCESS(result001))
		RhinoApp().Print(L"Failed at\t ovr_Initialize()\n");

	HINSTANCE hinst = GetModuleHandle(nullptr); // thanks b to stackoverflow. gets current HINSTANCE of whatever exe is running...
	// this one from OGL. need to open window, need HINSTANCE from WinMain...
	VALIDATE(Platform.InitWindow(hinst, L"VRAPTOR"), "Failed to open window.");
}

void glErrorReport()
{
	// this function is actually... not straightforward. https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
			GLenum glError = glGetError();
			switch(glError)
			{
			case GL_NO_ERROR:
				{
					RhinoApp().Print(L"GLNOERROR\n");
				}
			case GL_INVALID_ENUM:
				{
					RhinoApp().Print(L"GLINVALIDENUM\n");
				}
			case GL_INVALID_VALUE:
				{
					RhinoApp().Print(L"GLINVALIDVALUE\n");
				}
			case GL_INVALID_OPERATION:
				{
					RhinoApp().Print(L"GLINVALIDOPERATION\n");
				}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				{
					RhinoApp().Print(L"GLINVALIDFRAMEBUFFEROPERATION\n");
				}
			case GL_OUT_OF_MEMORY:
				{
					RhinoApp().Print(L"GLOUTOFMEMORY\n");
				}
			case GL_STACK_UNDERFLOW:
				{
					RhinoApp().Print(L"GLSTACKUNDERFLOW\n");
				}
			case GL_STACK_OVERFLOW:
				{
					RhinoApp().Print(L"GLSTACKOVERFLOW\n");
				}
			}
}

GLuint rhinoTex; // is tex 
GLuint mortyTex;
GLuint readBufferTexMort;
GLuint drawBufferTexMort;
GLsizei num = 1;

void glCleanAndBindBuffers()
{
	glBindTexture(GL_TEXTURE_2D, 0); // unbinds all tex so our code doesn't fux with anything else

	// also make swap framebuffers
	glGenFramebuffers(num, &readBufferTexMort);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readBufferTexMort); 
	GLint rbStatus = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
	glGenFramebuffers(1, &drawBufferTexMort);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBufferTexMort);
}

void makeMortyTex() // 
{
	int soilW, soilH;
	unsigned char* morty = SOIL_load_image("D:/mortyAmorphy.png", &soilW, &soilH, 0, SOIL_LOAD_RGBA); // loading not werking
	if (morty == NULL)
	{
		RhinoApp().Print(L"MORTY IS NULL MORTY\n");
	}

	// make mortyTex & setup to be identical to ovr swapset textures.

	glGenTextures(1, &mortyTex); 
	glBindTexture(GL_TEXTURE_2D, mortyTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, soilW, soilH, 0, GL_RGBA, GL_UNSIGNED_BYTE, morty);  // must be rgba

	if (morty == NULL)
	{
		RhinoApp().Print(L"MORTY IS NULL MORTY AT THE SECOND ONE MORTY\n");
	}

} 

void makeRhinoTex() // 
{
	// CRhinoUiDib lDibAtBitmap;
	// CRhinoUiDib theDib;

	/* // trying to hit the right... sdk... pump... where does it come from
	VR().lView->ActiveViewport().DisplayPipeline()->GetFrameBuffer();
	VR().lView->ActiveViewport().DisplayPipeline()->GetFrameBuffer().Size();
	VR().lView->ActiveViewport().DisplayPipeline()->CopyFrameBufferTo(CDC *); // display context
	VR().lView->ActiveViewport().DisplayPipeline()->DrawToDC(); // check params
	VR().lView->ActiveViewport().DisplayPipeline()->DisplayAttrs(); // CRuntimeClass ?!
	CRuntimeClass engineEngine = VR().lView->ActiveViewport().DisplayPipeline()->Engine()->classCRhinoDisplayEngine;
	CDC theDC = VR().lView->ActiveViewport().DisplayPipeline()->Engine()->GetDC();
	VR().lView->GetRenderTarget();
	VR().lView->get
	*/ // need that turbulent juice

	/////////////////// BEGIN DIB DRAW
	// need to get pixels out of rhino dib and go glDrawPixels into framebuffer

	CRhinoView* theView = VR().lView;

	CRhinoDib theDib;

	if(theView)
	{
		// lDibAtBitmap = VR().lView->ActiveViewport().DisplayPipeline()->GetFrameBuffer().Bitmap(); 
			CRect rect;
			theView->GetClientRect(rect); // CWind something


			if (theDib.CreateDib(rect.Width(), rect.Height(), 32, true)) // watch -> 32 is color depth. was 24 in rhino example
			{
				// set these flags as you wish
				BOOL bIgnoreHighlights = FALSE;
				BOOL bDrawTitle = FALSE;
				BOOL bDrawConstructionPlane = TRUE;
				BOOL bDrawWorldAxes = TRUE;

				CRhinoObjectIterator it(CRhinoObjectIterator::normal_or_locked_objects, 
										CRhinoObjectIterator::active_and_reference_objects); // sweet class m8. dunno wutchu doin

				if(theView->ActiveViewport().DisplayMode() == ON::wireframe_display)
				{
					VR().VRLaunchContext->m_doc.DrawToDC(it, theDib, theDib.Width(), theDib.Height(),
						theView->ActiveViewport().View(),
						bIgnoreHighlights,
						bDrawTitle,
						bDrawConstructionPlane,
						bDrawWorldAxes
						);
				}
			}
	}

	// condragulations.
	// Figure out where does that view update bug come from? &onViewDc ?

	// there is a shitload of restructuring to do...



	GLsizei rhDibW = theDib.Width(); // rhfb rhinoframebuffer as in above
	GLsizei rhDibH = theDib.Height(); // OK just need to get the DIB to actually contain a viewport...

	LPCTSTR theDibFile = L"D:/theDib.bmp"; // lol it's BLACK black buds
	theDib.SaveBmp(theDibFile);

	COLORREF theNewColor = 0x00909090;
	// theDib.Clear(theNewColor);

	LPCTSTR theNewDib = L"D:/theNewDib.bmp";
	theDib.SaveBmp(theNewDib);

	/*
	infoBits->bmiColors; // RGBQuad definition...
	HBITMAP hbitmap = theDib.Bitmap(); // handle...
	UINT dibSize = theDib.GetStorageSize();
	int cDepth = theDib.ColorDepth(); // 32
	*/

	

	//////////////////////// BEGIN BITMAP UNDERSTANDING

	const BITMAPINFO *bmi = theDib.BitmapInfo(); 
	const BITMAPINFOHEADER bmih = bmi->bmiHeader; // header; in example this is at BITMAPFILEHEADER ?
	LPBYTE lpByte = theDib.FindDIBBits(); // literally, 0z and 1z. is probably LPVOID bmBits;
	WORD bitCount = bmih.biBitCount;

	long bHeight = bmih.biHeight;
	long bWidth = bmih.biWidth;

	int pointW;
	int pointH;

	int dataSize = (bWidth*bHeight*(unsigned int)(bmih.biBitCount/8.0));

/*
	unsigned char* newRhinoTexData = SOIL_load_image_from_memory(lpByte, dataSize, &pointW, &pointH, 0, SOIL_LOAD_RGBA);

	int inspectWer = pointW;
	int inspectHer = pointH;
	int whatis = dataSize;
*/

/*
	int dataSize = (bWidth*bHeight*(unsigned int)(bmih.biBitCount/8.0));

	int dataSizeSize = sizeof(dataSize); // 1 102 156

	// I believe this is just the pixel data: no header

	int sizeBmih = sizeof(bmih); // 40-> this means that we have a bitmap

	int sizeBitCount = sizeof(bitCount); // bitCount... 32. same as colour depth

	int sizeBitCompression = sizeof(bmih.biCompression); // compression is 0 so no compression

 	int sizeByte = sizeof(lpByte); // 8 (ptr) // array of bytes? wtf?
	int sizeByteAnd = sizeof(&lpByte); // 8 (ptr)

	int sizeHeader = sizeof(bmi->bmiColors); // 4 (4 units in array 4 rgbquad)
	int sizeHeaderZero = sizeof(bmi->bmiColors[12]); // 4. wtf. u thro no error

	////////////////////////// BEGIN BITMAP HANDLING

	//byteWidth is the width of the actual image in bytes
	//padWidth is the width of the image plus the extra padding
	LONG byteWidth,padWidth;

	//initially set both to the width of the image
	byteWidth=padWidth=(LONG)((float)bmih.biWidth*(float)bmih.biBitCount/8.0); // bmih.biBitCount was bpp in example; declared previously, had some if statements...

	//add any extra space to bring each line to a DWORD boundary
	while(padWidth%4!=0) {
		padWidth++;
	}
	

	/////////////////////////// BEGIN BIT BY BIT BYTES

	DWORD diff;
	int offset;

	diff = bHeight*byteWidth;
	
	BYTE *pixelData = new BYTE[diff]; // but anyways I think this is just lpByte. Gambling.

	//bitmap is inverted, so the padding needs to be removed
	//and the image reversed
	//Here you can start from the back of the file or the front,
	//after the header.  The only problem is that some programs
	//will pad not only the data, but also the file size to
	//be divisible by 4 bytes.

	if(bHeight>0)
	{
		offset = padWidth-byteWidth;
		for(int i=0; i<dataSize; i+=4) // have converted source example from 3->4 bc 24->32 bit (RGB->RGBA)
		{
			if((i+1%padWidth==0))
			{
				i+=offset;
			}
			*(pixelData+i+3)=*(lpByte+i);
			*(pixelData+i+2)=*(lpByte+i+1);
			*(pixelData+i+1)=*(lpByte+i+2);
			*(pixelData+i)=*(lpByte+i+3);
		}
	}

*/

	/* FOR ABOVE.... First the actual size of the image is determined and memory is allocated for it. 
	Then the height is checked to see if the image is reversed or not. 
	If the image is reversed, the data needs to be copied byte by byte into the final storage area. 
	If the image is not reversed, then we can make use of memcpy() to quickly move the data. 
	This is not a big deal as image loading should not occur in time critical code anyway. */



	// make rhinoTex & setup to be identical to ovr swapset textures.

	glBindTexture(GL_TEXTURE_2D, 0); // safe-t

	glGenTextures(1, &rhinoTex); 
	glBindTexture(GL_TEXTURE_2D, rhinoTex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // so that we don't combine with the original.. ?

	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, bWidth, bHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, lpByte);

/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// now that we have the RGBA Bits there is else to do; 
	// http://www.gamedev.net/page/resources/_/technical/game-programming/how-to-load-a-bitmap-r1966
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, bWidth, bHeight, 0, GL_RGBA, GL_UNSIGNED_CHAR, newRhinoTexData);  // THIS IS LIKELY WHERE THE ERROR IS
	// BYTES must not be coming in hot enough

	if (lpByte == NULL)
	{
		RhinoApp().Print(L"PIXELS ARE NULL MORTY AT THE SECOND ONE MORTY\n");
	}

*/

}


void CVRaptorPlugIn::HMDDisplayAnything()
{
	// Setup 4 OVR happiness
	ovrWinWomb();
	using namespace OVR;

	// END what was WINAPI

	// Begin MainLoop
    TextureBuffer * eyeRenderTexture[2] = { nullptr, nullptr };
    DepthBuffer   * eyeDepthBuffer[2] = { nullptr, nullptr };
    ovrGLTexture  * mirrorTexture = nullptr;
    GLuint          mirrorFBO = 0;
    Scene         * roomScene = nullptr; 

    ovrHmd HMD;
	ovrGraphicsLuid luid;
    ovrResult result002 = ovr_Create(&HMD, &luid);
    if (!OVR_SUCCESS(result002))
		RhinoApp().Print(L"Failed at\t ovr_Create\n");

    ovrHmdDesc hmdDesc = ovr_GetHmdDesc(HMD);

	ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };
    if (!Platform.InitDevice(windowSize.w, windowSize.h, reinterpret_cast<LUID*>(&luid))) // if it doesn't work: used to goto shutdown
		RhinoApp().Print(L"Failed at\t Platform.InitDevice\n");

	
	makeMortyTex(); // mortyTex;
	makeRhinoTex(); // rhinoTex; // SOMETHING HERE 334
	glCleanAndBindBuffers(); // unbinds GL_TEXTURE_2D and builds / binds framebuffers

	GLuint theTex = rhinoTex; // pick 4 debug

	// make input Texture Width & Height

	glBindTexture(GL_TEXTURE_2D, theTex);

	GLint theTexW, theTexH, theTexFormat;

	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &theTexW);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &theTexH);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &theTexFormat);
	bool isItTrue = false;
	isItTrue = glIsTexture(theTex); // HAS WIDTH, HAS HEIGHT, IS A TEXTURE

    // Make eye render buffers
    for (int eye = 0; eye < 2; ++eye)
    {
        ovrSizei idealTextureSize = ovr_GetFovTextureSize(HMD, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
        eyeRenderTexture[eye] = new TextureBuffer(HMD, true, true, idealTextureSize, 1, NULL, 1); // IS CAUSING THIS TO THROW 000z 316
        eyeDepthBuffer[eye]   = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);
		// dunwuntthis

        if (!eyeRenderTexture[eye]->TextureSet)
        {
            RhinoApp().Print(L"Failed at\t createEyRenderTexture TextureSet\n");
        }
    }

	// Create mirror texture and an FBO used to copy mirror texture to back buffer
    ovrResult resultOVRCREATE = ovr_CreateMirrorTextureGL(HMD, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h, reinterpret_cast<ovrTexture**>(&mirrorTexture));
    if (!OVR_SUCCESS(resultOVRCREATE))
    {
        RhinoApp().Print(L"Failed at\t ovr_CreateMirrorTextureGL()\n");
    }

	// Configure the MIRROR read buffer
    glGenFramebuffers(1, &mirrorFBO);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
    glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    ovrEyeRenderDesc EyeRenderDesc[2]; 
    EyeRenderDesc[0] = ovr_GetRenderDesc(HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
    EyeRenderDesc[1] = ovr_GetRenderDesc(HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

    // Turn off vsync to let the compositor do its magic
    wglSwapIntervalEXT(0);

    bool isVisible = true;
	
	/////////////////////////////////////////////////////////////////////////
	// I guess OGL calls Platform.HandleMessages() when something happens?
	// is this how we do our timing? could it b that e-z ?
	while (Platform.HandleMessages()) // Platform.HandleMessages()
    {
        // Keyboard inputs to adjust player orientation
        static float Yaw(3.141592f);  
        if (Platform.Key[VK_LEFT])  Yaw += 0.02f;
        if (Platform.Key[VK_RIGHT]) Yaw -= 0.02f;

        // Keyboard inputs to adjust player position
        static Vector3f Pos2(0.0f,1.6f,-5.0f);
        if (Platform.Key['W']||Platform.Key[VK_UP])     Pos2+=Matrix4f::RotationY(Yaw).Transform(Vector3f(0,0,-0.05f));
        if (Platform.Key['S']||Platform.Key[VK_DOWN])   Pos2+=Matrix4f::RotationY(Yaw).Transform(Vector3f(0,0,+0.05f));
        if (Platform.Key['D'])                          Pos2+=Matrix4f::RotationY(Yaw).Transform(Vector3f(+0.05f,0,0));
        if (Platform.Key['A'])                          Pos2+=Matrix4f::RotationY(Yaw).Transform(Vector3f(-0.05f,0,0));
        Pos2.y = ovr_GetFloat(HMD, OVR_KEY_EYE_HEIGHT, Pos2.y);

        // Get eye poses, feeding in correct IPD offset
        ovrVector3f               ViewOffset[2] = { EyeRenderDesc[0].HmdToEyeViewOffset,
                                                    EyeRenderDesc[1].HmdToEyeViewOffset };
        ovrPosef                  EyeRenderPose[2];

        double           ftiming = ovr_GetPredictedDisplayTime(HMD, 0);
        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
        double           sensorSampleTime = ovr_GetTimeInSeconds();
        ovrTrackingState hmdState = ovr_GetTrackingState(HMD, ftiming, ovrTrue);
        ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);

        if (isVisible)
        {
            for (int eye = 0; eye < 2; ++eye)
            {

                // Get view and projection matrices
                Matrix4f rollPitchYaw = Matrix4f::RotationY(Yaw);
                Matrix4f finalRollPitchYaw = rollPitchYaw * Matrix4f(EyeRenderPose[eye].Orientation);
                Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
                Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
                Vector3f shiftedEyePos = Pos2 + rollPitchYaw.Transform(EyeRenderPose[eye].Position);

                Matrix4f view = Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);
                Matrix4f proj = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[eye], 0.2f, 1000.0f, ovrProjection_RightHanded);

                // Increment to use next texture, just before writing
                eyeRenderTexture[eye]->TextureSet->CurrentIndex = (eyeRenderTexture[eye]->TextureSet->CurrentIndex + 1) % eyeRenderTexture[eye]->TextureSet->TextureCount;

				// Switch to eye render target (but we r not going 2 render 2 it bc we are just passing pixels
                eyeRenderTexture[eye]->SetAndClearRenderSurface(eyeDepthBuffer[eye]); // binds the current texture for rendering, so app can render into texture & pass texture to OVR

				// MOD

				// pull current Texture from OVR set
				ovrGLTexture* tex = (ovrGLTexture*)&eyeRenderTexture[eye]->TextureSet->Textures[eyeRenderTexture[eye]->TextureSet->CurrentIndex];

				// HAVE TO BLIT INTO FRAMEBUFFER. READ THE INTERNET. BUILD THE BUFFERS (they are already there). WRITE TO THE BUFFERS.
				// submitting via layer DOESN'T WORK OVR Y U GOTTA B SO OPAQUE

				GLboolean isIt = glIsTexture(tex->OGL.TexId);	// check OVR tex
				GLboolean isItReal = glIsTexture(theTex);		// check our tex

				glBindTexture(GL_TEXTURE_2D, tex->OGL.TexId);	// bind OVR tex

				// get OVR tex width / height / format

				GLint ovrTexW, ovrTexH, ovrTexFormat;
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &ovrTexW); 
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &ovrTexH);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &ovrTexFormat);
				

				// Set one Framebuffer as read, the other as write. can these be any framebuffers? 
				// are not GL_FRAMEBUFFER so different target than in SetAndClearRenderSurface, should be SAFE();
				glBindFramebuffer(GL_READ_FRAMEBUFFER, readBufferTexMort);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawBufferTexMort);

				// theTex is our input
				// tex->OGL.TexID is the Oculus texture from the texture set.
				glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, theTex, 0);
				glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->OGL.TexId, 0);
				// Do the copy.
				glBlitFramebuffer(0, 0, theTexW, theTexH, 0, 0, ovrTexW, ovrTexH, GL_COLOR_BUFFER_BIT, GL_NEAREST); // BLIT IT HOME MORTY

                // Avoids an error when calling SetAndClearRenderSurface during next iteration.
                // Without this, during the next while loop iteration SetAndClearRenderSurface
                // would bind a framebuffer with an invalid COLOR_ATTACHMENT0 because the texture ID
                // associated with COLOR_ATTACHMENT0 had been unlocked by calling wglDXUnlockObjectsNV
				
				eyeRenderTexture[eye]->UnsetRenderSurface(); // removes bindings from framebuffer. 0s.
            }
        }

        // Do distortion rendering, Present and flush/sync

        // Set up positional data.
        ovrViewScaleDesc viewScaleDesc;
        viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
        viewScaleDesc.HmdToEyeViewOffset[0] = ViewOffset[0];
        viewScaleDesc.HmdToEyeViewOffset[1] = ViewOffset[1];

        ovrLayerEyeFov ld;
        ld.Header.Type  = ovrLayerType_EyeFov;
        ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

        for (int eye = 0; eye < 2; ++eye)
        {
			// do textureSet debug here ?
            ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureSet; // DO IT LIKE THIS MORTY
            ld.Viewport[eye]     = Recti(eyeRenderTexture[eye]->GetSize());
            ld.Fov[eye]          = hmdDesc.DefaultEyeFov[eye];
            ld.RenderPose[eye]   = EyeRenderPose[eye];
            ld.SensorSampleTime  = sensorSampleTime;
        }

        ovrLayerHeader* layers = &ld.Header;
        ovrResult resultSubmit = ovr_SubmitFrame(HMD, 0, &viewScaleDesc, &layers, 1);
        // exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost
        if (!OVR_SUCCESS(resultSubmit))
            goto Done;

		// glErrorReport();

        // Blit mirror texture to back buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        GLint w = mirrorTexture->OGL.Header.TextureSize.w;
        GLint h = mirrorTexture->OGL.Header.TextureSize.h;
        glBlitFramebuffer(0, h, w, 0,
                          0, 0, w, h,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        SwapBuffers(Platform.hDC);
    }

	/////////////// END MAINLOOP

Done:
    delete roomScene;
    if (mirrorFBO) glDeleteFramebuffers(1, &mirrorFBO);
    if (mirrorTexture) ovr_DestroyMirrorTexture(HMD, reinterpret_cast<ovrTexture*>(mirrorTexture));
    for (int eye = 0; eye < 2; ++eye)
    {
        delete eyeRenderTexture[eye];
        delete eyeDepthBuffer[eye];
    }
    Platform.ReleaseDevice();
    ovr_Destroy(HMD);

	RhinoApp().Print(L"\n //////////////////////////////////////// FIN DRAWANYTHING\n");

    // Retry on ovrError_DisplayLost
    //return retryCreate || OVR_SUCCESS(result) || (result == ovrError_DisplayLost);


}

void CVRaptorPlugIn::HMDRender() //copies current lView and rView buffers to ovr TextureSet and submits frame
{
	// PLACEHOLDER. NO IDEA WHAT IS GOING TO GO HERE
	VR().lView->DisplayPipeline()->GetFrameBuffer();
	VR().rView->DisplayPipeline()->GetFrameBuffer();

	ovrResult       result = ovr_SubmitFrame(hmdSession, 0, nullptr, 0, 0);
}

void CVRaptorPlugIn::HMDPrintUpdate()
{
	// code here to print all relevant data from class, for debugging
}

void CVRaptorPlugIn::HMDDestroy()
{
	ovr_Destroy(hmdSession);
	ovr_Shutdown();

	ON_wString wStr;
	wStr.Format( L"HMDDestroy.\n" );
	RhinoApp().Print( wStr );
}

///////////
// Runtime

ovrTrackingState CVRaptorPlugIn::OVRDoTracking()	// in the future we should set this up to take an absTime function for 
										// ovr_GetTrackingState to pull
{
		// Query HMD for current tracking state
	ts = ovr_GetTrackingState(hmdSession, 0.0, false); 
			// 2nd arg, abstime, defines what absolute system time we want a reading for. 0.0 for most recent reading, where 'predicted pose' and 'sample pose' will be identical.
			// 3rd arg has to do with latency timing for debugging, when true a timer starts from here -> to measure 'app-to-mid-photon' time. 
	if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked ))
	{
		// straight to eye poses, big TY to ovr for this offsets maths.
		ovr_CalcEyePoses(ts.HeadPose.ThePose, hmdToEyeViewOffsetRaptor, tsEyePoses); // yall got 0's in your offset vector

		// setup all relevant values to do next moves
		// already have all things in ts and tsEyePoses
		eyePoseQuats[0] = tsEyePoses[0].Orientation; // quaternion class for rotation angle, used later
		eyePoseQuats[1] = tsEyePoses[1].Orientation;

	}
	else
	{
		RhinoApp().Print(L"Unable to poll Oculus Tracking: Please open Oculus Configuration Utility and try again\n");
	}

	return ts;
}

void CVRaptorPlugIn::OVRtoRHCams(ovrPosef pose[2]) // there pose[2] is tsEyePoses 
{
	for (int i = 0; i< 2; i++)
	{
		// ok, take pose and decomp into location
		// camLocation[]
		camLoc[i] = scaleMult * ON_3dPoint(pose[i].Position.x, pose[i].Position.y, pose[i].Position.z);

		rotationVector = eyePoseQuats[i].ToRotationVector();

		//RhinoApp().Print( L"OVRtoRHCams rotationVector = \t %f \t %f \t %f \n", rotationVector.x, rotationVector.y, rotationVector.x);

		dirBase = ON_3dVector(0.0, 1.0, 0.0);
		upBase = ON_3dVector(0.0, 0.0, 1.0);

		dirBase.Rotate( rotationVector.Length() , ON_3dVector( rotationVector.x, rotationVector.y, rotationVector.z) );
		upBase.Rotate( rotationVector.Length() , ON_3dVector( rotationVector.x, rotationVector.y, rotationVector.z) );

		camDir[i] = dirBase;
		camUp[i] = upBase; // there's a chance dir and up can be called only once; given parallel eyes

		//RhinoApp().Print( L"camDir[i] = \t\t\t %f \t %f \t %f \n", camDir[i].x, camDir[i].y, camDir[i].z );
		//RhinoApp().Print( L"camup[i] = \t\t\t %f \t %f \t %f \n", camUp[i].x, camUp[i].y, camUp[i].z );

		// then setcams (2nd for, so simulteneous)
		// then redraw
	}

	lView->ActiveViewport().m_v.m_vp.SetCameraLocation(camLoc[0]);
	lView->ActiveViewport().m_v.m_vp.SetCameraDirection(camDir[0]);
	lView->ActiveViewport().m_v.m_vp.SetCameraUp(camUp[0]);

	rView->ActiveViewport().m_v.m_vp.SetCameraLocation(camLoc[1]);
	rView->ActiveViewport().m_v.m_vp.SetCameraDirection(camDir[1]);
	rView->ActiveViewport().m_v.m_vp.SetCameraUp(camUp[1]);
	
	lView->Redraw(); // so we will be re-calling immediately -> maybe no bueno
	rView->Redraw();
}

void CVRaptorPlugIn::HMDViewsUpdate()
{
	OVRDoTracking(); // to update all vals
	OVRtoRHCams(tsEyePoses);
	//RhinoApp().Wait(16); // so approximately 60fps; that's not a lot of milliseconds! 
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CVRaptorPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CVRaptorPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CVRaptorPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {295D51BF-1B63-455A-9331-CD3E593494AB}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CVRaptorPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CVRaptorPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CVRaptorPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return TRUE;
}

BOOL CVRaptorPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp( hWnd );
}

