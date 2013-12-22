// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofx/Media/AVStreamInfo.h"


namespace ofx {
namespace Media {


AVStreamInfo::AVStreamInfo():
    id(-1),
    codecID(AV_CODEC_ID_NONE),
    codecName("unknown"),
    codecLongName("unknown"),
    codecProfile("unknown"),
    codecProperties(0),
    level(-1),
    codecTag("unknown"),
    streamCodecTag("unknown"),
    averageBitRate(-1),

    duration(-1),
    startTime(-1),
    numFrames(-1),

    audioNumChannels(-1),
    audioSampleRate(-1),
    audioBitsPerSample(-1),

    videoWidth(0),
    videoHeight(0),
    videoFrameRate(0),
    videoHasBFrames(false),
    videoPixelFormatDescriptor("unknown"),
    videoDecodedFormat("unknown")
{
    averageFrameRate.num = -1;
    averageFrameRate.den = -1;

    timeBase.num = -1;
    timeBase.den = -1;

    videoSampleAspectRatio.num = -1;
    videoSampleAspectRatio.den = -1;

    videoDisplayAspectRatio.num = -1;
    videoDisplayAspectRatio.den = -1;
}

AVStreamInfo::~AVStreamInfo()
{
}


} } // namespace ofx::Media
