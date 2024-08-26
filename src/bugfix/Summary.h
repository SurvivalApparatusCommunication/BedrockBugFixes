#pragma once

#include <memory>

namespace bbf {
struct HopperBugsFix {
    struct Impl;
    std::unique_ptr<Impl> impl;

    void call(bool);
    HopperBugsFix();
    ~HopperBugsFix();
};
struct BeeBugsFix {
    struct Impl;
    std::unique_ptr<Impl> impl;

    void call(bool);
    BeeBugsFix();
    ~BeeBugsFix();
};
struct AllayTeleportBugFix {
    struct Impl;
    std::unique_ptr<Impl> impl;

    void call(bool);
    AllayTeleportBugFix();
    ~AllayTeleportBugFix();
};
struct ChunkSaveBugFix {
    struct Impl;
    std::unique_ptr<Impl> impl;

    void call(bool);
    ChunkSaveBugFix();
    ~ChunkSaveBugFix();
};
} // namespace bbf
