const std = @import("std");

const input = @embedFile("input.txt");

pub fn U32(n: anytype) u32 {
    return @as(u32, @intCast(n));
}

pub fn I64(n: anytype) i64 {
    return @as(i64, @intCast(n));
}

pub fn USZ(n: anytype) usize {
    return @as(usize, @intCast(n));
}

pub fn sqr(a: i64, b: i64) i64 {
    return std.math.powi(i64, a, b) catch unreachable;
}

const Vec = struct {
    x: i64, y: i64, z: i64, i: u32,

    pub fn init(x: i64, y: i64, z: i64, i: u32) Vec {
        return .{
            .x = x, .y = y, .z = z, .i = i,
        };
    }

    pub fn dist(self: Vec, other: Vec) i64 {
        return sqr(self.x - other.x, 2)
            + sqr(self.y - other.y, 2)
            + sqr(self.z - other.z, 2);
    }

    pub fn print(self: Vec) void {
        std.debug.print("({}, {}, {})\n", .{self.x, self.y, self.z});
    }
};

pub fn main() void {
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    const alloc = arena.allocator();

    var coords = std.ArrayList(Vec).initCapacity(alloc, 1000) catch unreachable;
    defer coords.deinit(alloc);

    {
        var i: u32 = 0;
        while (i < input.len) {
            var v = Vec.init(0, 0, 0, U32(coords.items.len));
            while (input[i] != ',') { v.x = v.x * 10 + input[i] - '0'; i += 1; }
            i += 1;
            while (input[i] != ',') { v.y = v.y * 10 + input[i] - '0'; i += 1; }
            i += 1;
            while (input[i] != '\n') { v.z = v.z * 10 + input[i] - '0'; i += 1; }
            i += 1;
            coords.append(alloc, v) catch unreachable;
        }
    }

    // Iterate pairs
    var pairs = std.ArrayList([3]i64).initCapacity(alloc, coords.items.len) catch unreachable;
    pairs.insert(alloc, 0, .{
        coords.items[0].dist(coords.items[1]), 0, 1
    }) catch unreachable;

    for (coords.items, 0..) |a, i| {
        for (coords.items[i+1..], i+1..) |b, j| {
            // Insert pair into list
            const dist = a.dist(b);
            pairs.append(alloc, .{dist, I64(i), I64(j)}) catch unreachable;
        }
    }

    std.mem.sort([3]i64, pairs.items, {}, struct {
        pub fn inner(_: void, a: [3]i64, b: [3]i64) bool {
            return a[0] < b[0];
        }
    }.inner);

    // for (pairs.items) |p| {
    //     std.debug.print("Pair: {}-{}, dist: {}\n", .{p[1], p[2], p[0]});
    // }

    // Iterate pairs closest to farthest, if not already in same group, move b to a's group
    var nSets: usize = coords.items.len;
    var last: [2]Vec = undefined;
    for (pairs.items) |p| {
        if (coords.items[USZ(p[1])].i == coords.items[USZ(p[2])].i) continue;

        nSets -= 1; // Merging removes 1 set

        // Move b to a's group
        const aGrp = coords.items[USZ(p[1])].i;
        const bGrp = coords.items[USZ(p[2])].i;

        if (nSets == 1) {
            std.debug.print("Last sets: {}, {}\n", .{aGrp, bGrp});
            last[0] = coords.items[USZ(p[1])];
            last[1] = coords.items[USZ(p[2])];
            last[0].print();
            last[1].print();
        }

        std.debug.print("Merging: {} and {}\n", .{aGrp, bGrp});
        for (coords.items) |*c| {
            if (c.i == bGrp) c.i = aGrp;
        }
    }

    std.debug.print("Answer: {}\n", .{last[0].x * last[1].x});
}
