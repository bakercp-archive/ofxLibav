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


#include "ofx/Media/AVInfo.h"
#include <math.h>


extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixdesc.h"
}

#include <time.h> 


namespace ofx {
namespace Media {


static void print_error(const char *filename, int errorCode)
{
    char errorBuffer[128];
    const char* errorBufferPtr = errorBuffer;
    
    if (av_strerror(errorCode, errorBuffer, sizeof(errorBuffer)) < 0)
    {
        errorBufferPtr = strerror(AVUNERROR(errorCode));
    }
    
    ofLogError("ofxAVProbe") <<  filename << " : " << errorBufferPtr;
}

    
AVMediaInfo AVProbe::probe(const std::string& path)
{

    AVMediaInfo info;

    info.path = ofToDataPath(path, true);

    av_register_all();

    AVFormatContext* avFormatContext = 0;
    
    int error = -1;
    
    error = avformat_open_input(&avFormatContext,
                                info.path.toString().c_str(),
                                0,  // we are not asking for a specific format
                                0); // we are not providing specific format details

    if(error < 0)
    {
        print_error(info.path.toString().c_str(),error);
        return info;
    }
    
    // fill the streams in the format context 
    error = avformat_find_stream_info(avFormatContext, 0);

    if(error < 0)
    {
        print_error(info.path.toString().c_str(),error);
        return info;
    }

    // add file-wide metadata
    if(avFormatContext->metadata)
    {
        AVDictionaryEntry *t = 0;
        while((t = av_dict_get(avFormatContext->metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
        {
            info.metadata.add(t->key, t->value);
        }
    }

    // bind a decoder to each input stream
    for(std::size_t i = 0; i < avFormatContext->nb_streams; i++)
    {

        AVStream* stream = avFormatContext->streams[i];
        AVCodec*  codec  = 0;

        if (stream->codec->codec_id == AV_CODEC_ID_PROBE)
        {
            ofLogWarning("ofxAVProbe::probe") << "Failed to probe codec for input stream " << stream->index;
        }
        else
        {
            codec = avcodec_find_decoder(stream->codec->codec_id);

            if(!codec)
            {
                ofLogWarning("ofxAVProbe::probe") << "Unsupported codec with id " << stream->codec->codec_id << " for input stream " << stream->index;
            }
            else
            {
                error = avcodec_open2(stream->codec, codec, 0);

                if(error < 0)
                {
                    ofLogWarning("ofxAVProbe::probe") << "Error while opening codec for input stream for input stream " << stream->index;
                }
            }
        }
    }

    
    for (std::size_t streamIdx = 0; streamIdx < avFormatContext->nb_streams; streamIdx++)
    {
        ofx::Media::AVStreamInfo avStream;

        AVStream* stream = avFormatContext->streams[streamIdx];
        const AVCodec* dec;

        const char* profile;
        char val_str[128];
        AVRational displayAspectRatio;
        AVRational* sampleAspectRatio = 0;

        avStream.averageFrameRate = stream->avg_frame_rate;
        avStream.averageBitRate = stream->codec->bit_rate;
        avStream.timeBase = stream->time_base;
        avStream.startTime = stream->start_time;
        avStream.duration = stream->duration;
        avStream.numFrames = stream->nb_frames;

        if (stream->codec)
        {
            dec = stream->codec->codec;

            if (dec)
            {
                avStream.codecName = std::string(dec->name);
                avStream.codecLongName = std::string(dec->long_name);

                profile = av_get_profile_name(dec, stream->codec->profile);

                if(profile)
                {
                    avStream.codecProfile = profile;
                }

            }

            // encoding level
            avStream.level = stream->codec->level;

            // codec type
            avStream.codecType = stream->codec->codec_type;

            // codec time base
            avStream.timeBase = stream->codec->time_base; // time increment per frame in seconds

            // codec tag
            av_get_codec_tag_string(val_str, sizeof(val_str), stream->codec->codec_tag);
            avStream.codecTag = val_str;

            av_get_codec_tag_string(val_str, sizeof(val_str), stream->codec->stream_codec_tag);
            avStream.streamCodecTag = val_str;


            switch (stream->codec->codec_type)
            {
                case AVMEDIA_TYPE_UNKNOWN:       ///< Usually treated as AVMEDIA_TYPE_DATA
                case AVMEDIA_TYPE_DATA:          ///< Opaque data information usually continuous
                case AVMEDIA_TYPE_SUBTITLE:
                case AVMEDIA_TYPE_ATTACHMENT:    ///< Opaque data information usually sparse
                case AVMEDIA_TYPE_NB:
                case AVMEDIA_TYPE_VIDEO:
                {
                    // compression
                    avStream.videoHasBFrames = stream->codec->has_b_frames;

                    // image dimensions
                    avStream.videoWidth = stream->codec->width;
                    avStream.videoHeight = stream->codec->height;

                    bool hasSampleAspectRatio = false;

                    if(stream->codec->sample_aspect_ratio.num != 0)
                    {
                        avStream.videoSampleAspectRatio = stream->codec->sample_aspect_ratio;
                        hasSampleAspectRatio = true;
                    }
                    else if (stream->sample_aspect_ratio.num != 0)
                    {
                        avStream.videoSampleAspectRatio = stream->sample_aspect_ratio;
                        hasSampleAspectRatio = true;
                    }
                    
                    if (hasSampleAspectRatio)
                    {
                        // reduce
                        av_reduce(&avStream.videoDisplayAspectRatio.num,
                                  &avStream.videoDisplayAspectRatio.den,
                                  avStream.videoWidth  * avStream.videoSampleAspectRatio.num,
                                  avStream.videoHeight * avStream.videoSampleAspectRatio.den,
                                  1024*1024);
                    }
                    
                    const AVPixFmtDescriptor* desc = av_pix_fmt_desc_get(stream->codec->pix_fmt);

                    if(desc)
                    {
                        avStream.videoPixelFormatDescriptor = desc->name;
                    }

                    break;
                }
                case AVMEDIA_TYPE_AUDIO:
                {
                    // fs, Hz
                    avStream.audioSampleRate = stream->codec->sample_rate;

                    // # audio channels
                    avStream.audioNumChannels = stream->codec->channels;

                    // bits per sample
                    avStream.audioBitsPerSample = av_get_bits_per_sample(stream->codec->codec_id);
                    
                    break;
                }
            }
        }
        
        if (avFormatContext->iformat->flags & AVFMT_SHOW_IDS)
        {
            avStream.id = stream->id;
        }


        if(stream->metadata)
        {
             AVDictionaryEntry *t = 0;

             while((t = av_dict_get(stream->metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
             {
                avStream.metadata.add(t->key,t->value);
             }
        }

        // add the stream info
        info.streams.push_back(avStream);
    }

    // close decoder for each stream
    for (std::size_t i = 0; i < avFormatContext->nb_streams; i++)
    {
        avcodec_close(avFormatContext->streams[i]->codec);
    }
    
    // close input
    avformat_close_input(&avFormatContext);

    // return!
    return info;

}


} } // namespace ofx::Media
