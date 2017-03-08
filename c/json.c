
// ॐ //

#include <stdio.h>

#include "json.h"

// print string
void parse_string(UJObject str) {
    if (str == NULL || UJIsNull(str))
        printf("NULL");
    if (str == NULL)
        return;

    size_t         len  = 0;
    const wchar_t* wstr = UJReadString(str, &len);
    /*if(len == 0) {
        printf("-\t");
        return;
        }*/
    wprintf(L"%S\t", wstr);
}

// print int value
void parse_int(UJObject i) {
    if (i == NULL)
        return;

    wprintf(L"%d\t", UJNumericInt(i));
}

// parse user
void parse_user(UJObject user) {

    if (user == NULL)
        return;

    const wchar_t* keys[] = {L"uid", L"first_name", L"last_name",
                             L"sex", L"city",       L"country"};
    UJObject uid = NULL, first_name = NULL, last_name = NULL, sex = NULL,
             city = NULL, country = NULL;
    UJObjectUnpack(user, 6, "NSSNNN", keys, &uid, &first_name, &last_name, &sex,
                   &city, &country);
    parse_int(uid);
    parse_string(first_name);
    parse_string(last_name);
    parse_int(sex);
    parse_int(city);
    parse_int(country);

    wprintf(L"\n");
}

// parse json
void parse_json(buf_t* buf) {
    void*          state;
    const wchar_t* keys[] = {L"response", L"error"};
    UJObject       json = NULL, response = NULL, error = NULL;

    // printf("%s\n", buf->buf);

    json = UJDecode(buf->buf, buf->size, NULL, &state);
    if (json != NULL) {
        int cnt = UJObjectUnpack(json, 2, "AO", keys, &response, &error);
        if (error != NULL) {
        }
        if (response != NULL && UJGetType(response) == UJT_Array) {
            void*    iter = UJBeginArray(response);
            UJObject user = NULL;
            while (UJIterArray(&iter, &user)) {
                parse_user(user);
            }
        }
    }
}
