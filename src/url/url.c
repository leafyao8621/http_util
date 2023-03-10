#include "url.h"
#include "../util/errcode.h"
#include "../util/coder.h"

int URL_initialize(URL *url) {
    if (!url) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    int ret = DArrayChar_initialize(&url->text, 100);
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    ret = DArrayString_initialize(&url->breakdown, 10);
    if (ret) {
        return HTTP_UTIL_ERR_INITIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}

int URL_parse(URL *url, char **iter) {
    if (!url) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    if (**iter != '/') {
        return HTTP_UTIL_ILL_FORMATTED;
    }
    int ret = DArrayChar_push_back(&url->text, *iter);
    if (ret) {
        return HTTP_UTIL_ERR_PARSE;
    }
    char chr = 0;
    String buf, encoded;
    ret = DArrayChar_initialize(&buf, 100);
    if (ret) {
        return HTTP_UTIL_ERR_PARSE;
    }
    ret = DArrayChar_initialize(&encoded, 100);
    if (ret) {
        return HTTP_UTIL_ERR_PARSE;
    }
    for (++(*iter); **iter && **iter != ' ' && **iter != '?'; ++(*iter)) {
        int ret = DArrayChar_push_back(&url->text, *iter);
        if (ret) {
            return HTTP_UTIL_ERR_PARSE;
        }
        if (**iter == '/') {
            ret = DArrayChar_push_back(&buf, &chr);
            if (ret) {
                DArrayChar_finalize(&buf);
                DArrayChar_finalize(&encoded);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = url_decode(buf.data, &encoded);
            if (ret) {
                return ret;
            }
            ret = DArrayString_push_back(&url->breakdown, &encoded);
            if (ret) {
                DArrayChar_finalize(&buf);
                DArrayChar_finalize(&encoded);
                return HTTP_UTIL_ERR_PARSE;
            }
            DArrayChar_finalize(&buf);
            ret = DArrayChar_initialize(&buf, 100);
            if (ret) {
                DArrayChar_finalize(&encoded);
                return HTTP_UTIL_ERR_PARSE;
            }
            ret = DArrayChar_initialize(&encoded, 100);
            if (ret) {
                DArrayChar_finalize(&buf);
                DArrayChar_finalize(&encoded);
                return HTTP_UTIL_ERR_PARSE;
            }
        } else {
            ret = DArrayChar_push_back(&buf, *iter);
            if (ret) {
                DArrayChar_finalize(&buf);
                DArrayChar_finalize(&encoded);
                return HTTP_UTIL_ERR_PARSE;
            }
        }
    }
    if (!**iter) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&encoded);
        return HTTP_UTIL_ILL_FORMATTED;
    }
    if (buf.size) {
        ret = DArrayChar_push_back_batch(&url->text, "/", 2);
        if (ret) {
            DArrayChar_finalize(&buf);
            DArrayChar_finalize(&encoded);
            return HTTP_UTIL_ERR_PARSE;
        }
        ret = DArrayChar_push_back(&buf, &chr);
        if (ret) {
            DArrayChar_finalize(&buf);
            DArrayChar_finalize(&encoded);
            return HTTP_UTIL_ERR_PARSE;
        }
        ret = url_decode(buf.data, &encoded);
        if (ret) {
            return ret;
        }
        ret = DArrayString_push_back(&url->breakdown, &encoded);
        if (ret) {
            DArrayChar_finalize(&buf);
            DArrayChar_finalize(&encoded);
            return HTTP_UTIL_ERR_PARSE;
        }
        DArrayChar_finalize(&buf);
    } else {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&encoded);
        ret = DArrayChar_push_back(&url->text, &chr);
        if (ret) {
            return HTTP_UTIL_ERR_PARSE;
        }
    }
    return HTTP_UTIL_ERR_OK;
}

int URL_finalize(URL *url) {
    if (!url) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    DArrayChar_finalize(&url->text);
    String *iter = url->breakdown.data;
    for (size_t i = 0; i < url->breakdown.size; ++i, ++iter) {
        DArrayChar_finalize(iter);
    }
    DArrayString_finalize(&url->breakdown);
    return HTTP_UTIL_ERR_OK;
}

int URL_serialize(URL *url, String *buf) {
    if (!url || !buf) {
        return HTTP_UTIL_ERR_NULL_PTR;
    }
    char chr = '\n';
    int ret =
        DArrayChar_push_back_batch(buf, url->text.data, url->text.size - 1);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    String *iter = url->breakdown.data;
    for (size_t i = 0; i < url->breakdown.size; ++i, ++iter) {
        ret = DArrayChar_push_back_batch(buf, iter->data, iter->size - 1);
        if (ret) {
            return HTTP_UTIL_SERIALIZE;
        }
        ret = DArrayChar_push_back(buf, &chr);
        if (ret) {
            return HTTP_UTIL_SERIALIZE;
        }
    }
    chr = 0;
    ret = DArrayChar_push_back(buf, &chr);
    if (ret) {
        return HTTP_UTIL_SERIALIZE;
    }
    return HTTP_UTIL_ERR_OK;
}
