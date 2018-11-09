// Copyright 2015-2018 Josh Pieper, jjp@pobox.com.  All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <array>

#include "mj_assert.h"
#include "async_types.h"
#include "static_function.h"

/// This class manages exclusive ownership of a resource with
/// asynchronous semantics.
template <typename T, size_t MaxLockers = 3>
class AsyncExclusive {
 public:
  /// @p resource is aliased internally, and subsequently passed to
  /// each operation when it is ready to be started.
  AsyncExclusive(T* resource) : resource_(resource) {}

  using Operation = StaticFunction<void (T*, VoidCallback)>;

  /// Invoke @p operation when the resource is next available.  It
  /// will be passed a callback that can be used to relinquish
  /// ownership.
  void AsyncStart(const Operation& operation) {
    if (!outstanding_) {
      outstanding_ = true;
      operation(resource_, [this]() {
          this->outstanding_ = false;
          this->MaybeStartQueued();
        });
      return;
    }

    // Try to queue it.
    for (auto& item : callbacks_) {
      if (!item.valid()) {
        item = operation;
        return;
      }
    }

    // We had too many things trying to send at the same time.
    MJ_ASSERT(false);
  }

 private:
  void MaybeStartQueued() {
    MJ_ASSERT(!outstanding_);

    for (auto& item : callbacks_) {
      if (item.valid()) {
        // We'll do this one.
        auto copy = item;
        item = {};
        AsyncStart(copy);
        return;
      }
    }

    // Guess we don't have any outstanding callbacks.
  }

  T* const resource_;

  // Is there an outstanding operation?
  bool outstanding_ = false;
  std::array<Operation, MaxLockers> callbacks_;
};
