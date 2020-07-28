# -*- python -*-

# Copyright 2018-2020 Josh Pieper, jjp@pobox.com.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

load("//tools/workspace/bazel_deps:repository.bzl", "bazel_deps_repository")
load("//tools/workspace/bazel:repository.bzl", "bazel_repository")
load("//tools/workspace/clipp:repository.bzl", "clipp_repository")
load("//tools/workspace/function2:repository.bzl", "function2_repository")
load("//tools/workspace/rules_mbed:repository.bzl", "rules_mbed_repository")
load("//tools/workspace/rules_nodejs:repository.bzl", "rules_nodejs_repository")

def add_default_repositories(excludes = []):
    if "com_github_mjbots_rules_mbed" not in excludes:
        rules_mbed_repository()
    if "bazel" not in excludes:
        bazel_repository()
    if "bazel_deps" not in excludes:
        bazel_deps_repository(name = "com_github_mjbots_bazel_deps")
    if "clipp" not in excludes:
        clipp_repository(name = "com_github_muellan_clipp")
    if "build_bazel_rules_nodejs" not in excludes:
        rules_nodejs_repository()
    if "function2" not in excludes:
        function2_repository()
