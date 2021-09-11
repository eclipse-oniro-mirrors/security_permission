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

#include <gtest/gtest.h>
#include <fstream>

#include <set>
#include <vector>
#include "constant.h"
#include "remote_command.h"
#include "remote_protocol.h"

#include "base_remote_command.h"

using namespace testing::ext;
using namespace OHOS::Security::Permission;

namespace {}  // namespace

class BaseRemoteCommandTest : public testing::Test {
public:
    BaseRemoteCommandTest();
    ~BaseRemoteCommandTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    const std::shared_ptr<BaseRemoteCommand> GetBaseRemoteCommand() const;

    UidBundleBo GetDefaultInfo() const;

private:
    std::shared_ptr<BaseRemoteCommand> dataMgr_ = std::make_shared<BaseRemoteCommand>();
};

BaseRemoteCommandTest::BaseRemoteCommandTest()
{}
BaseRemoteCommandTest::~BaseRemoteCommandTest()
{}
void BaseRemoteCommandTest::SetUpTestCase()
{}
void BaseRemoteCommandTest::TearDownTestCase()
{}
void BaseRemoteCommandTest::SetUp()
{}
void BaseRemoteCommandTest::TearDown()
{}
const std::shared_ptr<BaseRemoteCommand> BaseRemoteCommandTest::GetBaseRemoteCommand() const
{
    return dataMgr_;
}

UidBundleBo BaseRemoteCommandTest::GetDefaultInfo() const
{

    BundlePermissionsDto bundles;
    bundles.name = "bundles.name";
    bundles.bundleLabel = "bundles.bundleLabel";
    SignDto signDto;
    signDto.sha256 = "bundles.sign";
    bundles.sign.push_back(signDto);
    std::vector<PermissionDto> permissions;

    for (int i = 1; i <= 5; i++) {
        PermissionDto permission;
        permission.permissionName = "permission.permissionName" + std::to_string(i);
        permission.bundleName = "permission.bundleName" + std::to_string(i);
        permission.grantMode = i;
        permission.availableScope = i;
        permission.label = "permission.label" + std::to_string(i);
        permission.labelId = i;
        permission.description = "permission.description" + std::to_string(i);
        permission.descriptionId = i;
        permissions.insert(permission);
    }

    UidBundleBo uidBundleBo;
    uidBundleBo.DEFAULT_SIZE = 10;         // default 10
    uidBundleBo.MIN_UID_PACKAGES_NUM = 1;  // default 1
    uidBundleBo.uid = 1;
    uidBundleBo.appAttribute = -2;
    uidBundleBo.bundles = bundles;
    uidBundleBo.uidState = 1;

    std::set<std::string> remoteSensitivePermission;
    for (int i = 1; i <= 5; i++) {
        remoteSensitivePermission.insert("permission.permissionName" + std::to_string(i));
    }
    return uidBundleBo;
}

UidBundleBo BaseRemoteCommandTest::GetNullPermissionsInfo() const
{

    BundlePermissionsDto bundles;
    bundles.name = "bundles.name";
    bundles.bundleLabel = "bundles.bundleLabel";

    SignDto signDto;
    signDto.sha256 = "bundles.sign";
    bundles.sign.push_back(signDto);

    std::vector<PermissionDto> permissions;

    UidBundleBo uidBundleBo;
    uidBundleBo.DEFAULT_SIZE = 10;         // default 10
    uidBundleBo.MIN_UID_PACKAGES_NUM = 1;  // default 1
    uidBundleBo.uid = 1;
    uidBundleBo.appAttribute = -2;
    uidBundleBo.bundles = bundles;
    uidBundleBo.uidState = 1;

    std::set<std::string> remoteSensitivePermission;
    for (int i = 1; i <= 5; i++) {
        remoteSensitivePermission.insert("permission.permissionName" + std::to_string(i));
    }
    return uidBundleBo;
}

UidBundleBo BaseRemoteCommandTest::GetNullRemoteSensitivePermissionsInfo() const
{

    BundlePermissionsDto bundles;
    bundles.name = "bundles.name";
    bundles.bundleLabel = "bundles.bundleLabel";
    SignDto signDto;
    signDto.sha256 = "bundles.sign";
    bundles.sign.push_back(signDto);
    std::vector<PermissionDto> permissions;

    uidBundleBo uidBundleBo;
    uidBundleBo.DEFAULT_SIZE = 10;         // default 10
    uidBundleBo.MIN_UID_PACKAGES_NUM = 1;  // default 1
    uidBundleBo.uid = 1;
    uidBundleBo.appAttribute = -2;
    uidBundleBo.bundles = bundles;
    uidBundleBo.uidState = 1;

    return uidBundleBo;
}

HWTEST_F(BaseRemoteCommandTest, BaseRemoteCommandTest_0003, Function | SmallTest | Level0)
{
    auto dataMgr = GetBaseRemoteCommand();
    ASSERT_NE(dataMgr, nullptr);

    const std::string CAMERA = "permission.CAMERA";
    const std::string fail_str = "permission.unknowpermission";

    UidBundleBo info = GetNullRemoteSensitivePermissionsInfo();

    dataMgr->GrantSensitivePermissionToRemoteApp(info, CAMERA);
    EXPECT_EQ(info.remoteSensitivePermission, result);

    for (const std::string &x : info.remoteSensitivePermission)
        EXPECT_EQ(x, CAMERA);

    dataMgr->GrantSensitivePermissionToRemoteApp(info, fail_str);
}

HWTEST_F(BaseRemoteCommandTest, BaseRemoteCommandTest_0002, Function | SmallTest | Level0)
{
    auto dataMgr = GetBaseRemoteCommand();
    ASSERT_NE(dataMgr, nullptr);

    const std::string CAMERA = "permission.CAMERA";
    const std::string HEALTH_SENSOR = "permission.BODY_SENSORS";
    const std::string LOCATION = "permission.ACCESS_FINE_LOCATION";
    const std::string MICROPHONE = "permission.RECORD_AUDIO";

    bool ret1 = dataMgr->IsSensitiveResource(CAMERA);
    bool ret2 = dataMgr->IsSensitiveResource(HEALTH_SENSOR);
    bool ret3 = dataMgr->IsSensitiveResource(LOCATION);
    bool ret4 = dataMgr->IsSensitiveResource(MICROPHONE);
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(ret2);
    EXPECT_TRUE(ret3);
    EXPECT_TRUE(ret4);

    const std::string fail_str = "permission.unknowpermission";
    bool ret5 = dataMgr->IsSensitiveResource(fail_str);
    EXPECT_TRUE(!ret5);
}

HWTEST_F(BaseRemoteCommandTest, BaseRemoteCommandTest_0001, Function | SmallTest | Level0)
{
    auto dataMgr = GetBaseRemoteCommand();
    ASSERT_NE(dataMgr, nullptr);
    bool ret1 = dataMgr->IsValid(GetDefaultInfo());
    bool ret2 = dataMgr->IsValid(GetNullPermissionsInfo());
    EXPECT_TRUE(ret1);
    EXPECT_TRUE(!ret2);
}

// /**
//  * @tc.number: UpdateInstallState_0200
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_START->UPDATING_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0200, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
// }

// /**
//  * @tc.number: UpdateInstallState_0300
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_START->UPDATING_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0300, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_0400
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_START->UNINSTALL_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0400, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
// }

// /**
//  * @tc.number: UpdateInstallState_0500
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_START->UNINSTALL_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0500, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
// }

// /**
//  * @tc.number: UpdateInstallState_0600
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_START
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0600, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_0700
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->UNINSTALL_START
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0700, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_0800
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->UNINSTALL_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0800, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_0900
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->UNINSTALL_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_0900, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_1000
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->UPDATING_STAR
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1000, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_1100
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->UPDATING_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1100, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_1200
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->UPDATING_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1200, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_FALSE(ret2);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
// }

// /**
//  * @tc.number: UpdateInstallState_1300
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->INSTALL_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1300, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_1400
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->INSTALL_START
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1400, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_1500
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->INSTALL_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1500, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_1600
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1600, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_1700
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UNINSTALL_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1700, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_1800
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_FAIL
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1800, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_FAIL);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_1900
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. NOT correct status transfer INSTALL_START->INSTALL_SUCCESS->UPDATING_SUCCESS
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_1900, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_FALSE(ret3);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: UpdateInstallState_2000
//  * @tc.name: UpdateInstallState
//  * @tc.desc: 1. empty bundle name
//  *           2. verify function return value
//  */
// HWTEST_F(BmsDataMgrTest, UpdateInstallState_2000, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState("", InstallState::INSTALL_START);
//     EXPECT_FALSE(ret1);
// }

// /**
//  * @tc.number: LoadDataFromPersistentStorage_0100
//  * @tc.name: LoadDataFromPersistentStorage
//  * @tc.desc: 1. NA
//  *           2. load data from persistent storage
//  */
// HWTEST_F(BmsDataMgrTest, LoadDataFromPersistentStorage_0100, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->LoadDataFromPersistentStorage();
//     EXPECT_FALSE(ret1);

//     InnerBundleInfo info;
//     BundleInfo bundleInfo;
//     bundleInfo.name = BUNDLE_NAME;
//     bundleInfo.applicationInfo.name = APP_NAME;
//     ApplicationInfo applicationInfo;
//     applicationInfo.name = BUNDLE_NAME;
//     info.SetBaseBundleInfo(bundleInfo);
//     info.SetBaseApplicationInfo(applicationInfo);

//     std::shared_ptr<IBundleDataStorage> dataStorage_ = std::make_shared<BundleDataStorage>();
//     dataStorage_->SaveStorageBundleInfo(Constants::CURRENT_DEVICE_ID, info);

//     bool ret2 = dataMgr->LoadDataFromPersistentStorage();
//     EXPECT_TRUE(ret2);
// }

// /**
//  * @tc.number: AddBundleInfo_0100
//  * @tc.name: AddBundleInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, AddBundleInfo_0100, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info;
//     BundleInfo bundleInfo;
//     bundleInfo.name = BUNDLE_NAME;
//     bundleInfo.applicationInfo.name = APP_NAME;
//     ApplicationInfo applicationInfo;
//     applicationInfo.name = BUNDLE_NAME;
//     applicationInfo.deviceId = DEVICE_ID;
//     info.SetBaseBundleInfo(bundleInfo);
//     info.SetBaseApplicationInfo(applicationInfo);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     InnerBundleInfo info1;
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
//     bool ret4 = dataMgr->GetInnerBundleInfo(BUNDLE_NAME, DEVICE_ID, info1);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: AddBundleInfo_0200
//  * @tc.name: AddBundleInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, AddBundleInfo_0200, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info1;
//     BundleInfo bundleInfo1;
//     bundleInfo1.name = BUNDLE_NAME;
//     bundleInfo1.applicationInfo.name = APP_NAME;
//     bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
//     ApplicationInfo applicationInfo1;
//     applicationInfo1.name = BUNDLE_NAME;
//     applicationInfo1.deviceId = DEVICE_ID;
//     info1.SetBaseBundleInfo(bundleInfo1);
//     info1.SetBaseApplicationInfo(applicationInfo1);

//     InnerBundleInfo info2;
//     BundleInfo bundleInfo2;
//     bundleInfo2.name = BUNDLE_NAME;
//     bundleInfo2.applicationInfo.name = APP_NAME;
//     bundleInfo2.applicationInfo.bundleName = BUNDLE_NAME;
//     ApplicationInfo applicationInfo2;
//     applicationInfo2.name = BUNDLE_NAME;
//     applicationInfo2.deviceId = DEVICE_ID;
//     info2.SetBaseBundleInfo(bundleInfo2);
//     info2.SetBaseApplicationInfo(applicationInfo2);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
//     bool ret5 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
//     bool ret6 = dataMgr->UpdateInnerBundleInfo(BUNDLE_NAME, info2, info1);
//     bool ret7 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
//     EXPECT_TRUE(ret5);
//     EXPECT_TRUE(ret6);
//     EXPECT_TRUE(ret7);

//     ApplicationInfo appInfo;
//     bool ret8 = dataMgr->GetApplicationInfo(BUNDLE_NAME, ApplicationFlag::GET_BASIC_APPLICATION_INFO, USERID,
//     appInfo); EXPECT_TRUE(ret8);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: AddBundleInfo_0300
//  * @tc.name: AddBundleInfo
//  * @tc.desc: 1. scan dir not exist
//  *           2. verify scan result file number is 0
//  */
// HWTEST_F(BmsDataMgrTest, AddBundleInfo_0300, Function | SmallTest | Level0)
// {
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     InnerBundleInfo info;
//     bool ret = dataMgr->AddInnerBundleInfo("", info);
//     EXPECT_FALSE(ret);
// }

// /**
//  * @tc.number: AddBundleInfo_0400
//  * @tc.name: AddBundleInfo
//  * @tc.desc: 1. add info to the data manager, then uninstall, then reinstall
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, AddBundleInfo_0400, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info;
//     BundleInfo bundleInfo;
//     bundleInfo.name = BUNDLE_NAME;
//     bundleInfo.applicationInfo.name = APP_NAME;
//     ApplicationInfo applicationInfo;
//     applicationInfo.name = BUNDLE_NAME;
//     applicationInfo.deviceId = DEVICE_ID;
//     info.SetBaseBundleInfo(bundleInfo);
//     info.SetBaseApplicationInfo(applicationInfo);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret5 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret6 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
//     EXPECT_TRUE(ret4);
//     EXPECT_TRUE(ret5);
//     EXPECT_TRUE(ret6);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: AddBundleInfo_0500
//  * @tc.name: AddBundleInfo
//  * @tc.desc: 1. add module info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, AddBundleInfo_0500, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info1;
//     BundleInfo bundleInfo1;
//     bundleInfo1.name = BUNDLE_NAME;
//     bundleInfo1.applicationInfo.name = APP_NAME;
//     bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
//     ApplicationInfo applicationInfo1;
//     applicationInfo1.name = BUNDLE_NAME;
//     applicationInfo1.deviceId = DEVICE_ID;
//     info1.SetBaseBundleInfo(bundleInfo1);
//     info1.SetBaseApplicationInfo(applicationInfo1);

//     InnerBundleInfo info2;
//     BundleInfo bundleInfo2;
//     bundleInfo2.name = BUNDLE_NAME;
//     bundleInfo2.applicationInfo.name = APP_NAME;
//     bundleInfo2.applicationInfo.bundleName = BUNDLE_NAME;
//     ApplicationInfo applicationInfo2;
//     applicationInfo2.name = BUNDLE_NAME;
//     applicationInfo2.deviceId = DEVICE_ID;
//     info2.SetBaseBundleInfo(bundleInfo2);
//     info2.SetBaseApplicationInfo(applicationInfo2);

//     InnerBundleInfo info3;
//     InnerBundleInfo info4;
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);
//     bool ret4 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
//     bool ret5 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_SUCCESS);
//     bool ret6 = dataMgr->AddNewModuleInfo(BUNDLE_NAME, info2, info1);
//     bool ret7 = dataMgr->GetInnerBundleInfo(BUNDLE_NAME, DEVICE_ID, info3);
//     bool ret8 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret9 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UPDATING_START);
//     bool ret10 = dataMgr->RemoveModuleInfo(BUNDLE_NAME, PACKAGE_NAME, info1);
//     bool ret11 = dataMgr->GetInnerBundleInfo(BUNDLE_NAME, DEVICE_ID, info4);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
//     EXPECT_TRUE(ret5);
//     EXPECT_TRUE(ret6);
//     EXPECT_TRUE(ret7);
//     EXPECT_TRUE(ret8);
//     EXPECT_TRUE(ret9);
//     EXPECT_TRUE(ret10);
//     EXPECT_TRUE(ret11);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: GenerateUidAndGid_0100
//  * @tc.name: GenerateUidAndGid
//  * @tc.desc: 1. app type is system app
//  *           2. generate uid and gid then verify
//  */
// HWTEST_F(BmsDataMgrTest, GenerateUidAndGid_0100, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info;
//     BundleInfo bundleInfo;
//     bundleInfo.name = BUNDLE_NAME;
//     bundleInfo.applicationInfo.name = APP_NAME;
//     ApplicationInfo applicationInfo;
//     applicationInfo.name = BUNDLE_NAME;
//     applicationInfo.deviceId = DEVICE_ID;
//     info.SetBaseBundleInfo(bundleInfo);
//     info.SetBaseApplicationInfo(applicationInfo);
//     info.SetAppType(Constants::AppType::SYSTEM_APP);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
//     bool ret4 = dataMgr->GenerateUidAndGid(info);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: GenerateUidAndGid_0200
//  * @tc.name: GenerateUidAndGid
//  * @tc.desc: 1. app type is third party app
//  *           2. generate uid and gid then verify
//  */
// HWTEST_F(BmsDataMgrTest, GenerateUidAndGid_0200, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info;
//     BundleInfo bundleInfo;
//     bundleInfo.name = BUNDLE_NAME;
//     bundleInfo.applicationInfo.name = APP_NAME;
//     ApplicationInfo applicationInfo;
//     applicationInfo.name = BUNDLE_NAME;
//     applicationInfo.deviceId = DEVICE_ID;
//     info.SetBaseBundleInfo(bundleInfo);
//     info.SetBaseApplicationInfo(applicationInfo);
//     info.SetAppType(Constants::AppType::THIRD_SYSTEM_APP);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
//     bool ret4 = dataMgr->GenerateUidAndGid(info);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: GenerateUidAndGid_0300
//  * @tc.name: GenerateUidAndGid
//  * @tc.desc: 1. app type is third party app
//  *           2. generate uid and gid then verify
//  */
// HWTEST_F(BmsDataMgrTest, GenerateUidAndGid_0300, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info;
//     BundleInfo bundleInfo;
//     bundleInfo.name = BUNDLE_NAME;
//     bundleInfo.applicationInfo.name = APP_NAME;
//     ApplicationInfo applicationInfo;
//     applicationInfo.name = BUNDLE_NAME;
//     applicationInfo.deviceId = DEVICE_ID;
//     info.SetBaseBundleInfo(bundleInfo);
//     info.SetBaseApplicationInfo(applicationInfo);
//     info.SetAppType(Constants::AppType::THIRD_PARTY_APP);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info);
//     bool ret4 = dataMgr->GenerateUidAndGid(info);
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: QueryAbilityInfo_0100
//  * @tc.name: QueryAbilityInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, QueryAbilityInfo_0100, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info1;
//     BundleInfo bundleInfo1;
//     bundleInfo1.name = BUNDLE_NAME;
//     bundleInfo1.applicationInfo.name = APP_NAME;
//     bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
//     ApplicationInfo applicationInfo1;
//     applicationInfo1.name = BUNDLE_NAME;

//     AbilityInfo abilityInfo = GetDefaultAbilityInfo();
//     bundleInfo1.abilityInfos.push_back(abilityInfo);
//     info1.SetBaseBundleInfo(bundleInfo1);
//     info1.SetBaseApplicationInfo(applicationInfo1);
//     info1.InsertAbilitiesInfo(BUNDLE_NAME + PACKAGE_NAME + ABILITY_NAME, abilityInfo);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     EXPECT_TRUE(ret1);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     EXPECT_TRUE(ret2);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);
//     EXPECT_TRUE(ret3);

//     Want want;
//     ElementName name;
//     name.SetAbilityName(ABILITY_NAME);
//     name.SetBundleName(BUNDLE_NAME);
//     want.SetElement(name);

//     AbilityInfo abilityInfo2;
//     bool ret4 = dataMgr->QueryAbilityInfo(want, abilityInfo2);
//     EXPECT_TRUE(ret4);

//     Parcel parcel;
//     parcel.WriteParcelable(&abilityInfo);
//     std::unique_ptr<AbilityInfo> abilityInfo3;
//     abilityInfo3.reset(parcel.ReadParcelable<AbilityInfo>());

//     EXPECT_EQ(abilityInfo3->package, abilityInfo2.package);
//     EXPECT_EQ(abilityInfo3->name, abilityInfo2.name);
//     EXPECT_EQ(abilityInfo3->bundleName, abilityInfo2.bundleName);
//     EXPECT_EQ(abilityInfo3->applicationName, abilityInfo2.applicationName);
//     EXPECT_EQ(abilityInfo3->deviceId, abilityInfo2.deviceId);
//     EXPECT_EQ(abilityInfo3->label, abilityInfo2.label);
//     EXPECT_EQ(abilityInfo3->description, abilityInfo2.description);
//     EXPECT_EQ(abilityInfo3->iconPath, abilityInfo2.iconPath);
//     EXPECT_EQ(abilityInfo3->visible, abilityInfo2.visible);
//     EXPECT_EQ(abilityInfo3->kind, abilityInfo2.kind);
//     EXPECT_EQ(abilityInfo3->type, abilityInfo2.type);
//     EXPECT_EQ(abilityInfo3->orientation, abilityInfo2.orientation);
//     EXPECT_EQ(abilityInfo3->launchMode, abilityInfo2.launchMode);
//     EXPECT_EQ(abilityInfo3->codePath, abilityInfo2.codePath);
//     EXPECT_EQ(abilityInfo3->resourcePath, abilityInfo2.resourcePath);
//     EXPECT_EQ(abilityInfo3->libPath, abilityInfo2.libPath);

//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: QueryAbilityInfo_0200
//  * @tc.name: QueryAbilityInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, QueryAbilityInfo_0200, Function | SmallTest | Level0)
// {
//     Want want;
//     ElementName name;
//     name.SetAbilityName(ABILITY_NAME);
//     want.SetElement(name);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     AbilityInfo abilityInfo;
//     bool ret = dataMgr->QueryAbilityInfo(want, abilityInfo);
//     EXPECT_FALSE(ret);
// }

// /**
//  * @tc.number: QueryAbilityInfo_0300
//  * @tc.name: QueryAbilityInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, QueryAbilityInfo_0300, Function | SmallTest | Level0)
// {
//     Want want;
//     ElementName element1;
//     EXPECT_EQ("//", element1.GetURI());

//     element1.SetDeviceID(DEVICE_ID);
//     EXPECT_EQ(DEVICE_ID, element1.GetDeviceID());

//     element1.SetBundleName(BUNDLE_NAME);
//     EXPECT_EQ(BUNDLE_NAME, element1.GetBundleName());

//     element1.SetAbilityName(ABILITY_NAME);
//     EXPECT_EQ(ABILITY_NAME, element1.GetAbilityName());
//     EXPECT_EQ(DEVICE_ID + "/" + BUNDLE_NAME + "/" + ABILITY_NAME, element1.GetURI());

//     ElementName element2(DEVICE_ID, BUNDLE_NAME, ABILITY_NAME);
//     EXPECT_EQ(DEVICE_ID + "/" + BUNDLE_NAME + "/" + ABILITY_NAME, element2.GetURI());

//     bool equal = (element2 == element1);
//     EXPECT_TRUE(equal);

//     Parcel parcel;
//     parcel.WriteParcelable(&element1);
//     std::unique_ptr<ElementName> newElement;
//     newElement.reset(parcel.ReadParcelable<ElementName>());
//     EXPECT_EQ(newElement->GetDeviceID(), element1.GetDeviceID());
//     EXPECT_EQ(newElement->GetBundleName(), element1.GetBundleName());
//     EXPECT_EQ(newElement->GetAbilityName(), element1.GetAbilityName());

//     want.SetElement(element1);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     AbilityInfo abilityInfo;
//     bool ret = dataMgr->QueryAbilityInfo(want, abilityInfo);
//     EXPECT_FALSE(ret);
// }

// /**
//  * @tc.number: GetApplicationInfo_0100
//  * @tc.name: GetApplicationInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, GetApplicationInfo_0100, Function | SmallTest | Level0)
// {
//     InnerBundleInfo info1;
//     BundleInfo bundleInfo1;
//     bundleInfo1.name = BUNDLE_NAME;
//     bundleInfo1.applicationInfo.name = APP_NAME;
//     bundleInfo1.applicationInfo.bundleName = BUNDLE_NAME;
//     ApplicationInfo applicationInfo1;
//     applicationInfo1.name = BUNDLE_NAME;
//     info1.SetBaseBundleInfo(bundleInfo1);
//     info1.SetBaseApplicationInfo(applicationInfo1);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     bool ret1 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_START);
//     bool ret2 = dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::INSTALL_SUCCESS);
//     bool ret3 = dataMgr->AddInnerBundleInfo(BUNDLE_NAME, info1);

//     ApplicationInfo appInfo;
//     bool ret4 = dataMgr->GetApplicationInfo(APP_NAME, ApplicationFlag::GET_BASIC_APPLICATION_INFO, USERID, appInfo);
//     std::string name = appInfo.name;
//     EXPECT_TRUE(ret1);
//     EXPECT_TRUE(ret2);
//     EXPECT_TRUE(ret3);
//     EXPECT_TRUE(ret4);
//     EXPECT_EQ(name, APP_NAME);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_START);
//     dataMgr->UpdateBundleInstallState(BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
// }

// /**
//  * @tc.number: GetApplicationInfo_0200
//  * @tc.name: GetApplicationInfo
//  * @tc.desc: 1. add info to the data manager
//  *           2. query data then verify
//  */
// HWTEST_F(BmsDataMgrTest, GetApplicationInfo_0200, Function | SmallTest | Level0)
// {
//     ApplicationInfo appInfo;
//     appInfo.name = APP_NAME;
//     appInfo.bundleName = BUNDLE_NAME;
//     appInfo.deviceId = DEVICE_ID;

//     Parcel parcel;
//     parcel.WriteParcelable(&appInfo);
//     std::unique_ptr<ApplicationInfo> appInfo2;
//     appInfo2.reset(parcel.ReadParcelable<ApplicationInfo>());
//     EXPECT_EQ(appInfo.name, appInfo2->name);
//     EXPECT_EQ(appInfo.bundleName, appInfo2->bundleName);
//     EXPECT_EQ(appInfo.deviceId, appInfo2->deviceId);
//     auto dataMgr = GetDataMgr();
//     ASSERT_NE(dataMgr, nullptr);
//     ApplicationInfo appInfo3;
//     bool ret = dataMgr->GetApplicationInfo(BUNDLE_NAME, ApplicationFlag::GET_BASIC_APPLICATION_INFO, USERID,
//     appInfo3); EXPECT_FALSE(ret);

//     EXPECT_NE(appInfo.name, appInfo3.name);
//     EXPECT_NE(appInfo.bundleName, appInfo3.bundleName);
//     EXPECT_NE(appInfo.deviceId, appInfo3.deviceId);
// }