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

//
//#include <math.h>
//#include <ctime.h>
//
//extern "C"
//{
//#define __STDC_CONSTANT_MACROS // for UINT64_C
////#include <libavcodec/avcodec.h>
////#include <libavformat/avformat.h>
////#include <libswscale/swscale.h>
////#include <libavutil/pixdesc.h>
////#include <libavutil/time.h>
////#include "libavformat/avformat.h"
////#include "libavcodec/avcodec.h"
////#include "libavutil/opt.h"
////#include "libavutil/pixdesc.h"
////#include "libavutil/dict.h"
////#include "libavutil/libm.h"
////#include "libavdevice/avdevice.h"
//}



/*
static const char *const binary_unit_prefixes [] = { "", "Ki", "Mi", "Gi", "Ti", "Pi" };
static const char *const decimal_unit_prefixes[] = { "", "K" , "M" , "G" , "T" , "P"  };

static const char unit_second_str[]         = "s"    ;
static const char unit_hertz_str[]          = "Hz"   ;
static const char unit_byte_str[]           = "byte" ;
static const char unit_bit_per_second_str[] = "bit/s";

static char *value_string(char *buf, int buf_size, double val, const char *unit)
{
    
    int show_value_unit              = 1;
    int use_value_prefix             = 1;
    int use_byte_value_binary_prefix = 1;
    int use_value_sexagesimal_format = 1;

    
    if (unit == unit_second_str && use_value_sexagesimal_format) {
        double secs;
        int hours, mins;
        secs  = val;
        mins  = (int)secs / 60;
        secs  = secs - mins * 60;
        hours = mins / 60;
        mins %= 60;
        snprintf(buf, buf_size, "%d:%02d:%09.6f", hours, mins, secs);
    } else if (use_value_prefix) {
        const char *prefix_string;
        int index;
        
        if (unit == unit_byte_str && use_byte_value_binary_prefix) {
            index = (int) log2(val) / 10;
            index = av_clip(index, 0, FF_ARRAY_ELEMS(binary_unit_prefixes) - 1);
            val  /= powf(2, index * 10);
            prefix_string = binary_unit_prefixes[index];
        } else {
            index = (int) (log10(val)) / 3;
            index = av_clip(index, 0, FF_ARRAY_ELEMS(decimal_unit_prefixes) - 1);
            val  /= powf(10, index * 3);
            prefix_string = decimal_unit_prefixes[index];
        }
        snprintf(buf, buf_size, "%.*f%s%s",
                 index ? 3 : 0, val,
                 prefix_string,
                 show_value_unit ? unit : "");
    } else {
        snprintf(buf, buf_size, "%f%s", val, show_value_unit ? unit : "");
    }
    
    return buf;
}

static char *time_value_string(char *buf, int buf_size, int64_t val,
                               const AVRational *time_base)
{
    if (val == AV_NOPTS_VALUE) {
        snprintf(buf, buf_size, "N/A");
    } else {
        value_string(buf, buf_size, val * av_q2d(*time_base), unit_second_str);
    }
    
    return buf;
}

static char *ts_value_string(char *buf, int buf_size, int64_t ts)
{
    if (ts == AV_NOPTS_VALUE) {
        snprintf(buf, buf_size, "N/A");
    } else {
        snprintf(buf, buf_size, "%"PRId64, ts);
    }
    
    return buf;
}

static char *rational_string(char *buf, int buf_size, const char *sep, const AVRational *rat) {
    snprintf(buf, buf_size, "%d%s%d", rat->num, sep, rat->den);
    return buf;
}

static char *tag_string(char *buf, int buf_size, int tag) {
    snprintf(buf, buf_size, "0x%04x", tag);
    return buf;
}

static const char *media_type_string(enum AVMediaType media_type) {
    switch (media_type) {
        case AVMEDIA_TYPE_VIDEO:      return "video";
        case AVMEDIA_TYPE_AUDIO:      return "audio";
        case AVMEDIA_TYPE_DATA:       return "data";
        case AVMEDIA_TYPE_SUBTITLE:   return "subtitle";
        case AVMEDIA_TYPE_ATTACHMENT: return "attachment";
        default:                      return "unknown";
    }
}

static void print_error(const char *filename, int err)
{
    char errbuf[128];
    const char *errbuf_ptr = errbuf;
    
    if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
        errbuf_ptr = strerror(AVUNERROR(err));
    
    ofLogError("ofxAVProbe") <<  filename << " : " << *errbuf_ptr;
}


class MediaInfo
{
public:

//    Poco::Net::
    AVMediaType mediaType;
};

class ofxAVProbe {
public:
    
    static MediaInfo probe(const ofFile& file) {
        MediaInfo info;

//        uuid
//        uri
//        name
//        type
//        duration
//        frameCount
//        playcount
//        filehealth
//        filesize
//        size


//        result["UUID"] = 
//        result["URI"] = "hi";//file.path();

        
//        if(!file.exists()) {
//            ofLogError("ofxAVProbe::probe") << "File " << file.getAbsolutePath() << " does not exist.";
//            return result;
//        }

        std::string filename = file.getAbsolutePath();

        av_register_all();

        AVFormatContext* fmt_ctx = NULL;
        
        int err = -1;
        
        err = avformat_open_input(&fmt_ctx,
                                  filename.c_str(),
                                  NULL,  // we are not asking for a specific format
                                  NULL); // we are not providing specific format details

        if(err < 0) {
            print_error(filename.c_str(),err);
            return info;
        }
        
        // fill the streams in the format context 
        err = avformat_find_stream_info(fmt_ctx, NULL);
    
        if(err < 0) {
            print_error(filename.c_str(),err);
            return info;
        }

        // bind a decoder to each input stream
        for(std::size_t i = 0; i < fmt_ctx->nb_streams; i++)
        {
            AVStream* stream = fmt_ctx->streams[i];
            AVCodec*  codec  = NULL;

            if (stream->codec->codec_id == AV_CODEC_ID_PROBE) {
                ofLogWarning("ofxAVProbe::probe") << "Failed to probe codec for input stream " << stream->index;
            } else if (!(codec = avcodec_find_decoder(stream->codec->codec_id))) {
                ofLogWarning("ofxAVProbe::probe") << "Unsupported codec with id " << stream->codec->codec_id << " for input stream " << stream->index;
            } else if (avcodec_open2(stream->codec, codec, NULL) < 0) {
                ofLogWarning("ofxAVProbe::probe") << "Error while opening codec for input stream for input stream " << stream->index;
            }
        }
        
        for (size_t stream_idx = 0; stream_idx < fmt_ctx->nb_streams; stream_idx++) {
            
            AVStream *stream = fmt_ctx->streams[stream_idx];
            AVCodecContext *dec_ctx;
            const AVCodec *dec;
            const char *profile;
            char val_str[128];
            AVRational display_aspect_ratio;
            AVRational *sar = NULL;
            const AVPixFmtDescriptor *desc;
            
            if ((dec_ctx = stream->codec)) {
                if ((dec = dec_ctx->codec)) {
                    
                    cout << "codec_name=" << dec->name << endl;
                    cout << "codec_long_name=" << dec->long_name << endl;

                } else {
                    cout << "codec_name=" << "unknown" << endl;
                }
                
                cout << "codec_type=" << media_type_string(dec_ctx->codec_type) << endl;
                cout << "codec_time_base=" << rational_string(val_str, sizeof(val_str), "/", &dec_ctx->time_base) << endl;
                
                // print AVI/FourCC tag
                av_get_codec_tag_string(val_str, sizeof(val_str), dec_ctx->codec_tag);
                cout << "codec_tag_string=" << val_str << endl;
                cout << "codec_tag=" << tag_string(val_str, sizeof(val_str),dec_ctx->codec_tag) << endl;
                
//                // print profile, if there is one 
//                if (dec && (profile = av_get_profile_name(dec, dec_ctx->profile))) {
//                    cout << "profile=" << profile << endl;
//                }
//
//                switch (dec_ctx->codec_type) {
//                    case AVMEDIA_TYPE_UNKNOWN:       ///< Usually treated as AVMEDIA_TYPE_DATA
//                    case AVMEDIA_TYPE_DATA:          ///< Opaque data information usually continuous
//                    case AVMEDIA_TYPE_SUBTITLE:
//                    case AVMEDIA_TYPE_ATTACHMENT:    ///< Opaque data information usually sparse
//                    case AVMEDIA_TYPE_NB:
//                    case AVMEDIA_TYPE_VIDEO:
//                        cout << "width=" << dec_ctx->width << endl;
//                        cout << "height=" << dec_ctx->height << endl;
//                        cout << "has_b_frames=" << dec_ctx->has_b_frames << endl;
//                        if (dec_ctx->sample_aspect_ratio.num) {
//                            sar = &dec_ctx->sample_aspect_ratio;
//                        } else if (stream->sample_aspect_ratio.num) {
//                            sar = &stream->sample_aspect_ratio;
//                        }
//                        
//                        if (sar) {
//                            cout << "sample_aspect_ratio=" << rational_string(val_str, sizeof(val_str), ":", sar) << endl;
//                            av_reduce(&display_aspect_ratio.num, &display_aspect_ratio.den,
//                                      dec_ctx->width  * sar->num, dec_ctx->height * sar->den,
//                                      1024*1024);
//
//                            cout << "display_aspect_ratio=" << rational_string(val_str, sizeof(val_str), ":", &display_aspect_ratio) << endl;
//                        }
//                        
//                        desc = av_pix_fmt_desc_get(dec_ctx->pix_fmt);
//                        cout << "pix_fmt=" << (desc ? desc->name : "unknown") << endl;;
//                        cout << "level="   << (dec_ctx->level) << endl;
//                        break;
////                            
//                    case AVMEDIA_TYPE_AUDIO:
//                        cout << "sample_rate=" << value_string(val_str, sizeof(val_str),dec_ctx->sample_rate,unit_hertz_str) << endl;
//                        cout << "channels" << dec_ctx->channels << endl;
//                        cout << "bits_per_sample" << av_get_bits_per_sample(dec_ctx->codec_id) << endl;
//                        break;
//                }
            } else {
                cout << "codec_type=" << "unknown" << endl;
            }
            
            if (fmt_ctx->iformat->flags & AVFMT_SHOW_IDS)
                cout << "id=" << stream->id << endl;
            
            cout <<"avg_frame_rate="<< rational_string(val_str, sizeof(val_str), "/", &stream->avg_frame_rate) << endl;
           
            if (dec_ctx->bit_rate)
            {
                cout << "bit_rate=" << value_string(val_str, sizeof(val_str),dec_ctx->bit_rate, unit_bit_per_second_str) << endl;
            }

            cout << "time_base=" << rational_string(val_str, sizeof(val_str), "/",&stream->time_base) << endl;
            cout << "start_time=" << time_value_string(val_str, sizeof(val_str),stream->start_time, &stream->time_base) << endl;
            
            cout << "duration=" << time_value_string(val_str, sizeof(val_str),stream->duration, &stream->time_base) << endl;

            if (stream->nb_frames)
                cout << "nb_frames=" << stream->nb_frames << endl;
            
            cout << stream->metadata << "tags" << endl;
        
        }

        // close decoder for each stream
        for (int i = 0; i < fmt_ctx->nb_streams; i++) {
            AVStream *stream = fmt_ctx->streams[i];
            
            avcodec_close(stream->codec);
        }
        
        // close input
        avformat_close_input(&fmt_ctx);

    }

    ofThread
    
    
};

*/