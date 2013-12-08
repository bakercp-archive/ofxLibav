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


#include "ofApp.h"


//------------------------------------------------------------------------------
void ofApp::setup()
{
    // simulate a drag event for platforms that don't yet support drag events
    ofDragInfo simulatedDrag;
    simulatedDrag.files.push_back(ofToDataPath("0_fingers.mov"));
    ofNotifyDragEvent(simulatedDrag);
}

//------------------------------------------------------------------------------
void ofApp::update()
{
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);
    ofDrawBitmapStringHighlight("Drag a Media File Onto the Window:",
                                15,
                                25,
                                ofColor::white,
                                ofColor::black);

    ofSetColor(255);
    ofDrawBitmapString(displayString,15,45);
}

//------------------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
    MediaInfo info = AVProbe::probe(dragInfo.files[0]);

    std::stringstream ss;

    ss << "filename:" << " " << info.path.getFileName() << endl;

    for(size_t i = 0; i < info.streams.size(); i++)
    {
        ss << endl;
        ss << "stream (" << ofToString(i+1) << "/" << info.streams.size() << ")" << endl;
        ss << "=========================================================================" << endl;

        Stream stream = info.streams[i];

        if(stream.codecType == AVMEDIA_TYPE_VIDEO)
        {
            ss << setw(20) << "type:" << " " << "VIDEO STREAM" << endl;
            ss << setw(20) << "width:" << " " << stream.videoWidth << endl;
            ss << setw(20) << "height:" << " " << stream.videoHeight << endl;
            ss << setw(20) << "decoded format:" << " " << stream.videoDecodedFormat << endl;

        }
        else if(stream.codecType == AVMEDIA_TYPE_AUDIO)
        {
            ss << setw(20) << "type:" << " " << "AUDIO STREAM" << endl;
            ss << setw(20) << "num channels:" << " " << stream.audioNumChannels << endl;
            ss << setw(20) << "bits / sample:" << " " << stream.audioBitsPerSample << endl;
            ss << setw(20) << "sample rate:" << " " << stream.audioSampleRate << endl;

        }
        else
        {
        }

        ss << setw(20) << "codec:" << " " << stream.codecName << " [" << stream.codecLongName << "]" << endl;
        ss << setw(20) << "codec tag:" << " " << stream.codecTag << endl;
        ss << setw(20) << "stream codec tag:" << " " << stream.streamCodecTag << endl;
        ss << setw(20) << "profile:" << " " << stream.codecProfile << endl;
        ss << endl;

        ss << setw(20) << "avg. bitrate:" << " " << stream.averageBitRate << endl;
        ss << setw(20) << "avg. framerate:" << " " << (double)stream.averageFrameRate.num / stream.averageFrameRate.den << endl;

        ss << setw(20) << "Metadata:" << endl;

        Poco::Net::NameValueCollection::ConstIterator iter = stream.metadata.begin();
        while(iter != stream.metadata.end())
        {
            ss << setw(30) << iter->first << "=" << iter->second << endl;
            ++iter;
        }

    }

    displayString = ss.str();

}
