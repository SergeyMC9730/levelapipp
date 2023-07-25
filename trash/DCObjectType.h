#pragma once

enum DCObjectType {
    DCSubCommand = 1,
    DCSubCommandGroup = 2,
    DCString = 3,
    DCInteger = 4,
    DCBoolean = 5,
    DCUser = 6,
    DCChannel = 7,
    DCRole = 8,
    DCMentionable = 9,
    DCNumber = 10,
    DCAttachment = 11,
    DCommand = 0
};