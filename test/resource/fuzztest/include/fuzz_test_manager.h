/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef FUZZTESTMANAGER_H
#define FUZZTESTMANAGER_H

#include <memory>
#include <string>
#include <map>
#include <unordered_map>

#include "nlohmann/json.hpp"
namespace OHOS {
namespace Security {
namespace Permission {
class FuzzTestManager {
public:
    typedef std::shared_ptr<FuzzTestManager> Ptr;
    ~FuzzTestManager()
    {}
    static Ptr GetInstance()
    {
        if (instance_ == nullptr) {
            instance_ = std::shared_ptr<FuzzTestManager>(new FuzzTestManager);
        }
        return instance_;
    }
    void StartFuzzTest();

private:
    void SetJsonFunction(std::string);
    void SetCycle(uint16_t cycle);
    FuzzTestManager();
    FuzzTestManager(FuzzTestManager &) = delete;
    FuzzTestManager &operator=(const FuzzTestManager &) = delete;
    static Ptr instance_;
    uint16_t cycle_;
    std::unordered_map<std::string, int> remainderMap_;
    std::unordered_map<std::string, std::function<void()>> callFunctionMap_;

    void fuzzTestManagerTest();
    void DistributedPermissionKitFuzzTest();
};
}  // namespace Permission
}  // namespace Security
}  // namespace OHOS
#endif