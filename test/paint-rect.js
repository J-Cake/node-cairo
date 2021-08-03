
const lib = require('../cairo.node');

/// <path reference="./index.d.ts">
const surface = lib.create(1024, 768);
const paint = surface.rect([0xff, 0xff, 0xff], {x: 10, y: 10, width: 1024 - 20, height: 768 - 20});
