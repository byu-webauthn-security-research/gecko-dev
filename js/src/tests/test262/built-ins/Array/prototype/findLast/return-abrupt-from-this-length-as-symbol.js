// |reftest| shell-option(--enable-array-find-last) skip-if(!xulRuntime.shell) -- requires shell-options
// Copyright (C) 2021 Microsoft. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.
/*---
esid: sec-array.prototype.findlast
description: >
  Return abrupt from ToLength(Get(O, "length")) where length is a Symbol.
info: |
  Array.prototype.findLast ( predicate[ , thisArg ] )

  1. Let O be ? ToObject(this value).
  2. Let len be ? LengthOfArrayLike(O).

features: [Symbol, array-find-from-last]
---*/

var o = {};

o.length = Symbol(1);

// predicate fn is given to avoid false positives
assert.throws(TypeError, function() {
  [].findLast.call(o, function() {});
});

reportCompare(0, 0);
