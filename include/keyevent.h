
#pragma once

// this is in another header
// so that it can be included
// without attaching the whole 'application.h'

namespace glacier {
    
    typedef void (*KeyCharEventFunc) (const char character, void* userParam);

}
