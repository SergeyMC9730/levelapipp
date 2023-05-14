#pragma once

#define GET_JSON_VALUE(container, jsonval, memberval, type) if (container.contains(jsonval)) memberval = container[jsonval].get<type>();