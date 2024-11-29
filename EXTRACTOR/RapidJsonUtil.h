#pragma once
#include <StdAfx.h>
#include <map>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>


namespace assist
{
    namespace json
    {
        inline bool isNumberic(const std::string str)
        {
            try
            {
                std::stoi(str); // Try to convert the string to an integer
                return true;
            }
            catch (const std::invalid_argument& e)
            {
                return false; // Conversion to integer failed
            }
            catch (const std::out_of_range& e)
            {
                return false; // The string represents a number that's out of the integer range
            }
        }

        inline void parseJSON(const rapidjson::Value& value, std::map<CString, CString>& KeyValuePair, CString KEY)
        {

            if (value.IsObject())
            {
                // Process object members
                for (auto it = value.MemberBegin(); it != value.MemberEnd(); ++it)
                {
                    const char* key = it->name.GetString();
                    const rapidjson::Value& subValue = it->value;
                    parseJSON(subValue, KeyValuePair, (CString)key); // Recursively process sub-JSON
                }
            }
            else if (value.IsArray())
            {
                // Process array elements
                for (rapidjson::SizeType i = 0; i < value.Size(); ++i)
                {
                    const rapidjson::Value& subValue = value[i];
                    parseJSON(subValue, KeyValuePair, _T("")); // Recursively process sub-JSON
                }
            }
            else {
                // Process other value types (e.g., string, number, boolean)
                if (value.IsString())
                {
                    const char* str = value.GetString();
                    rapidjson::Document document;

                    // Check for parse errors
                    if (!document.Parse(str).HasParseError()) {

                        // POCO can parse a numberic string so this check needed
                        if (isNumberic(std::string(str)))
                        {
                            if (value.IsInt64())
                            {
                                INT64 intValue = value.GetInt64();
                                KeyValuePair[KEY].Format(_T("%lld"), intValue);
                            }
                            else if (value.IsDouble())
                            {
                                double doubleValue = value.GetDouble();
                                KeyValuePair[KEY].Format(_T("%Lf"), doubleValue);
                            }
                            else
                            {
                                KeyValuePair[KEY] = str;
                            }

                        }
                        else
                        {
                            rapidjson::Value& valueReference = document;
                            parseJSON(valueReference, KeyValuePair, _T(""));
                        }
                    }
                    else {
                        KeyValuePair[KEY] = str;
                    }
                }
                else if (value.IsNumber())
                {
                    if (value.IsInt64())
                    {
                        INT64 intValue = value.GetInt64();
                        KeyValuePair[KEY].Format(_T("%lld"), intValue);
                    }
                    else if (value.IsDouble())
                    {
                        double doubleValue = value.GetDouble();
                        KeyValuePair[KEY].Format(_T("%Lf"), doubleValue);
                    }
                }
                else if (value.IsBool())
                {
                    bool boolValue = value.GetBool();
                    KeyValuePair[KEY] = boolValue ? _T("true") : _T("false");
                }
                else
                {

                }
            }

        }
    }
}