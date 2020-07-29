#include "StaticResource.h"
#include <fstream>
#include <sys/stat.h>

using namespace std;


string to_lower(const string & s) {
    string ret = s;
    for(auto &i : ret) {
        if((i <= 'Z') && (i >= 'A')) i -= ('A' - 'a');
    }
    return ret;
}

void StaticResource::run(IHTTPRequest &req, IHTTPResponse &res, std::function<void()> next) {
    if(req.request.header.path.find(dest + "/") == 0) {
        string fn = source + "/" + req.request.header.path.substr(dest.length(), req.request.header.path.length());
        ifstream ifs;

        struct stat s_buf {};
        stat(fn.c_str(), &s_buf);

        if(!S_ISDIR(s_buf.st_mode)) {
            ifs.open(fn);
        } else {
            fn = fn + "/" + "index.html";
            ifs.open(fn);
        }

        if(ifs.good()) { // 文件存在，解析文件
            if(fn.find('.') == string::npos)
                res.setMIME({MIME_APPLICATION, "octet-stream"});
            else {
                const map<string, MIME_t> mMIMEExt = {
                        // 文本
                        {"html", {MIME_TEXT, "html"}},
                        {"htm", {MIME_TEXT, "html"}},
                        {"txt", {MIME_TEXT, "plain"}},
                        {"js", {MIME_TEXT, "javascript"}},
                        {"css", {MIME_TEXT, "css"}},
                        {"rtf", {MIME_TEXT, "rtf"}},
                        {"rtfd", {MIME_TEXT, "rtfd"}},
                        {"py", {MIME_TEXT, "x-python"}},
                        {"java", {MIME_TEXT, "x-java-source"}},
                        {"rb", {MIME_TEXT, "x-ruby"}},
                        {"sh", {MIME_TEXT, "x-shellscript"}},
                        {"pl", {MIME_TEXT, "x-perl"}},
                        {"sql", {MIME_TEXT, "x-sql"}},
                        // 图片
                        {"bmp", {MIME_IMAGE, "x-ms-bmp"}},
                        {"jpg", {MIME_IMAGE, "jpeg"}},
                        {"jpeg", {MIME_IMAGE, "jpeg"}},
                        {"gif", {MIME_IMAGE, "gif"}},
                        {"png", {MIME_IMAGE, "png"}},
                        {"tif", {MIME_IMAGE, "tiff"}},
                        {"tiff", {MIME_IMAGE, "tiff"}},
                        {"tga", {MIME_IMAGE, "x-targa"}},
                        {"psd", {MIME_IMAGE, "vnd.adobe.photoshop"}},
                        // 音频
                        {"mp3", {MIME_AUDIO, "mpeg"}},
                        {"mid", {MIME_AUDIO, "midi"}},
                        {"ogg", {MIME_AUDIO, "ogg"}},
                        {"mp4a", {MIME_AUDIO, "mp4"}},
                        {"wav", {MIME_AUDIO, "wav"}},
                        {"wma", {MIME_AUDIO, "x-ms-wma"}},
                        // 视频
                        {"avi", {MIME_VIDEO, "x-msvideo"}},
                        {"dv", {MIME_VIDEO, "x-dv"}},
                        {"mp4", {MIME_VIDEO, "mp4"}},
                        {"mpg", {MIME_VIDEO, "mpeg"}},
                        {"mpeg", {MIME_VIDEO, "mpeg"}},
                        {"mov", {MIME_VIDEO, "quicktime"}},
                        {"wm", {MIME_VIDEO, "x-ms-wmv"}},
                        {"wmv", {MIME_VIDEO, "x-ms-wmv"}},
                        {"flv", {MIME_VIDEO, "x-flv"}},
                        {"mkv", {MIME_VIDEO, "x-matroska"}},
                        // 文档
                        {"ai", {MIME_APPLICATION, "postscript"}},
                        {"eps", {MIME_APPLICATION, "postscript"}},
                        {"exe", {MIME_APPLICATION, "octet-stream"}},
                        {"doc", {MIME_APPLICATION, "vnd.ms-word"}},
                        {"docx", {MIME_APPLICATION, "vnd.ms-word"}},
                        {"xls", {MIME_APPLICATION, "vnd.ms-excel"}},
                        {"xlsx", {MIME_APPLICATION, "vnd.ms-excel"}},
                        {"ppt", {MIME_APPLICATION, "vnd.ms-powerpoint"}},
                        {"pps", {MIME_APPLICATION, "vnd.ms-powerpoint"}},
                        {"pptx", {MIME_APPLICATION, "vnd.ms-powerpoint"}},
                        {"ppsx", {MIME_APPLICATION, "vnd.ms-powerpoint"}},
                        {"pdf", {MIME_APPLICATION, "pdf"}},
                        {"xml", {MIME_APPLICATION, "xml"}},
                        {"odt", {MIME_APPLICATION, "vnd.oasis.opendocument.text"}},
                        {"swf", {MIME_APPLICATION, "x-shockwave-flash"}},
                };

                map<string, MIME_t>::const_iterator pos;
                auto ext = fn.substr(fn.find_last_of('.') + 1, fn.length());
                if((pos = mMIMEExt.find(to_lower(ext))) != mMIMEExt.end()) res.setMIME(pos->second);
                else res.setMIME({MIME_APPLICATION, "octet-stream"});
            }
            res.end(string((std::istreambuf_iterator<char>(ifs)),
                    std::istreambuf_iterator<char>()));
            return;
        }
    }
    next();
}
