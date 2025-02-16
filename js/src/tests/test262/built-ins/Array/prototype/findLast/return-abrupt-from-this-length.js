// |reftest| shell-option(--enable-array-find-last) skip-if(!xulRuntime.shell) -- requires shell-options
// Copyright (C) 2021 Microsoft. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.
/*---
esid: sec-array.prototype.findlast
description: >
  Return abrupt from ToLength(Get(O, "length")).
info: |
  Array.prototype.findLast ( predicate[ , thisArg ] )

  1. Let O be ? ToObject(this value).
  2. Let len be ? LengthOfArrayLike(O).
features: [array-find-from-last]
---*/

var o1 = {};

Object.defineProperty(o1, 'length', {
  get: function() {
    throw new Test262Error();
  },
  configurable: true
});

assert.throws(Test262Error, function() {
  [].findLast.call(o1);
});

var o2 = {
  length: {
    valueOf: function() {
      throw new Test262Error();
    }
  }
};
assert.throws(Test262Error, function() {
  [].findLast.call(o2);
});

reportCompare(0, 0);
