#pragma once

#include "cdif/cdif.h"

class FactoriesModule : public cdif::IModule {
    public:
        void load(cdif::Container& ctx) override;
};
