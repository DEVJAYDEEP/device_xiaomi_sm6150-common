/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <vector>

#include <libinit_utils.h>

using std::string;

void property_override(std::string prop, std::string value, bool add) {
    auto pi = (prop_info *) __system_property_find(prop.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), value.length());
    } else if (add) {
        __system_property_add(prop.c_str(), prop.length(), value.c_str(), value.length());
    }
}

std::vector<std::string> ro_props_default_source_order = {
    "odm.",
    "product.",
    "system.",
    "system_ext.",
    "vendor.",
    "",
};

void set_ro_build_prop(const std::string &prop, const std::string &value, bool product) {
    std::string prop_name;
    string safetynet = "green";

    string fp = "google/redfin/redfin:12/SPB5.210812.002/7671067:user/release-keys";
    string desc = "redfin-user 12 SPB5.210812.002 7671067 release-keys";

    for (const auto &source : ro_props_default_source_order) {
        if (product)
            prop_name = "ro.product." + source + prop;
        else
            prop_name = "ro." + source + "build." + prop;
        property_override(prop_name, value, true);
	property_override(string("ro.") + prop + string("build.fingerprint"), fp);
	property_override(string("ro.") + prop + string("build.description"), desc);
    }
    // Set safetynet workaround
    property_override("ro.boot.verifiedbootstate", safetynet);
    property_override(string("ro.build.fingerprint"), fp);
}
