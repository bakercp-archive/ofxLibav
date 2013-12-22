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


#pragma once


#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Net/NameValueCollection.h"


extern "C"
{
    #include "libavcodec/avcodec.h"
}


#include <time.h>

namespace ofx {
namespace Media {


class AVStreamInfo
{
public:
    AVStreamInfo();
    ~AVStreamInfo();

    int id;

    AVCodecID   codecID;
    AVMediaType codecType;
    std::string codecName;
    std::string codecLongName;
    std::string codecProfile;
    std::string codecTag;
    std::string streamCodecTag;
    int         codecProperties;
    int         level;

    int averageBitRate;
    AVRational averageFrameRate;
    AVRational timeBase;
    long long duration;
    long long startTime;
    long long numFrames;

    // audio
    int audioNumChannels;
    int audioSampleRate; // Hz
    int audioBitsPerSample;

    // video
    int videoWidth;
    int videoHeight;
    float videoFrameRate; // Hz
    bool videoHasBFrames;

    AVRational videoSampleAspectRatio;
    AVRational videoDisplayAspectRatio;
    std::string videoPixelFormatDescriptor; // numerical values are available

    std::string videoDecodedFormat; // ?

    Poco::Net::NameValueCollection metadata;
};


} } // namespace ofx::Media
