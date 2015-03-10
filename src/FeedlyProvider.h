#pragma once
#ifndef _FEEDLY_H
#define _FEEDLY_H

#include <curl/curl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#define DEFAULT_FCOUNT 500

struct UserData{
    std::map<std::string, std::string> categories;
    std::string id;
    std::string code;
    std::string authToken;
    std::string refreshToken;
    std::string galx;
};

struct PostData{
    std::string content;
    std::string title;
    std::string id;
    std::string originURL;
    std::string originTitle;
};

class FeedlyProvider{
    public:
        FeedlyProvider();
        PostData* getSinglePostData(const int index);
        void authenticateUser();
        void setVerbose(bool value);
        void setChangeTokensFlag(bool value);
        void curl_cleanup();
        bool markPostWithAction(const std::string &action, const std::vector<std::string>* ids);
        bool markCategoriesRead(const std::string& id, const std::string& lastReadEntryId);
        bool addSubscription(bool newCategory, const std::string& feed, std::vector<std::string> categories, const std::string& title = "");
        const std::vector<PostData>* givePostsFromStream(const std::string& category, bool whichRank = 0);
        const std::map<std::string, std::string>* getLabels();
        const std::string getUserId();
        const std::string getStreamId(const std::string& label);
        const std::map<std::string, unsigned int>* getUnreadCount(void);
    private:
        UserData user_data;
        CURL *curl;
        CURLcode curl_res;
        std::ofstream log_stream;
        std::string feedly_url;
        std::string userAuthCode;
        std::string TOKEN_PATH, TEMP_PATH, COOKIE_PATH, rtrv_count;
        std::vector<PostData> feeds;
        const std::string FEEDLY_URI = "https://cloud.feedly.com/v3/";
        long response_code = 0;
        bool verboseFlag, changeTokens, isLogStreamOpen=false;
        void getCookies();
        void enableVerbose();
        void curl_retrive(const std::string&);
        void curl_post(const std::string&, const std::string data);
        void extract_galx_value();
        void openLogStream();
};
#endif
