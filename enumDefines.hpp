#pragma once

enum argumentStructDefines {

    defualtHelpFlagVal,
    minPlayerCount,
    minPotatoCount = 1,
    defualtPotatoCountVal = 1,
    defualtPlayerCount = 4,
    maxPlayerCount = 32

};

enum argumentHandlerDefines {

    GetOptEOV = -1,
    handlerSuccessReturn,
    helpFlagSet,
    unknownArgGiven

};

enum argumentCheckerDefines {

    argStructPass,
    playerCountLessThen1Err,
    playerCountMoreThen32Err,
    potatoCountLessThen1Err,
    PotatoCountMoreThenPlayersErr
};