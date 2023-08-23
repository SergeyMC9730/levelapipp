/**
 *  LevelAPI - Geometry Dash level cacher with search functionality and more.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "iAndy.h"
#include "Translation.h"

#include <algorithm>

std::string iAndy::thousandSeparator(int n) {
    std::string ans = "";
    std::string num = std::to_string(n);
  
    // Initialise count
    int count = 0;
  
    // Traverse the string in reverse
    for (int i = num.size() - 1; i >= 0; i--) {
        count++;
        ans.push_back(num[i]);
  
        // If three characters
        // are traversed
        if (count == 3) {
            ans.push_back('.');
            count = 0;
        }
    }
  
    // Reverse the string to get
    // the desired output
    std::reverse(ans.begin(), ans.end());
  
    // If the given string is
    // less than 1000
    if (ans.size() % 4 == 0) {
        // Remove ','
        ans.erase(ans.begin());
    }
  
    return ans;
}

std::string iAndy::intToFormatString(int n, float &scaleMult) {
    if(n < 1000 || n > 100000000) return std::to_string(n);

    std::string str = thousandSeparator(n);
    str = str.substr(0, str.find(".") + 2);

    std::string suffix;

    if(n < 1000000) { 
        suffix = LevelAPI::Frontend::Translation::getByKey("iandy.prefix.k"); 
        scaleMult = 1.1f; 
    }
    else if(n < 100000000) { 
        suffix = LevelAPI::Frontend::Translation::getByKey("iandy.prefix.m"); 
        scaleMult = 1.5f; 
    }
	
    str += suffix;
	
	return str;
}