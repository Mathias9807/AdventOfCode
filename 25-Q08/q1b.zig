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
    // var gpa: std.heap.GeneralPurposeAllocator(.{}) = .{};
    // const alloc = gpa.allocator();
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

    // std.debug.print("Vec: {}\n", .{coords.items[0].z});

    // Iterate pairs
    var pairs = std.ArrayList([3]i64).initCapacity(alloc, coords.items.len) catch unreachable;
    pairs.insert(alloc, 0, .{
        coords.items[0].dist(coords.items[1]), 0, 1
    }) catch unreachable;

    // Hypothesis: There are not many extra pairs that link clusters that have already been linked
    // We don't need to iterate the entire list of pairs every time
    // We only care about the 1000 shortest after all
    const iterMaxBound = @as(u32, @intFromFloat(1000 * 1.5));

    for (coords.items, 0..) |a, i| {
        std.debug.print("test: {}\n", .{i});
        for (coords.items[i+1..], i+1..) |b, j| {
            // Insert pair into list at sorted position
            const dist = a.dist(b);
            for (pairs.items, 0..) |p, k| {
                if (dist < p[0]) {
                    pairs.insert(alloc, k, .{dist, I64(i), I64(j)}) catch unreachable;
                    break;
                }
                if (k > iterMaxBound) break;
            }
        }
    }

    for (pairs.items) |p| {
        std.debug.print("Pair: {}-{}, dist: {}\n", .{p[1], p[2], p[0]});
    }

    // coords.items[0].print();
    // coords.items[19].print();
    // std.debug.print("Dist: {}\n", .{coords.items[0].dist(coords.items[19])});

    // Iterate pairs closest to farthest, if not already in same group, move b to a's group
    for (pairs.items[0..1000]) |p| {
        if (coords.items[USZ(p[1])].i == coords.items[USZ(p[2])].i) continue;

        // Move b to a's group
        const aGrp = coords.items[USZ(p[1])].i;
        const bGrp = coords.items[USZ(p[2])].i;
        for (coords.items) |*c| {
            if (c.i == bGrp) c.i = aGrp;
        }
    }

    // Sum up the number of coords in each group
    var sum = alloc.alloc(u32, coords.items.len) catch unreachable;
    @memset(sum, 0);
    for (coords.items) |c| {
        sum[c.i] += 1;
    }

    for (sum, 0..) |grpCnt, g| {
        std.debug.print("Group: {}, count: {}\n", .{g, grpCnt});
    }

    // Find largest 3
    var largest = std.ArrayList(u32).initCapacity(alloc, 5) catch unreachable;
    for (sum) |grpCnt| {
        if (largest.items.len == 0 or grpCnt > largest.items[0]) {
            largest.insert(alloc, 0, grpCnt) catch unreachable;
        } else if (largest.items.len == 1 or grpCnt > largest.items[1]) {
            largest.insert(alloc, 1, grpCnt) catch unreachable;
        } else if (largest.items.len == 2 or grpCnt > largest.items[2]) {
            largest.insert(alloc, 2, grpCnt) catch unreachable;
        }
    }

    std.debug.print("Largest 3: {}, {}, {}\n", .{largest.items[0], largest.items[1], largest.items[2]});
    std.debug.print("Product: {}\n", .{largest.items[0] * largest.items[1] * largest.items[2]});
}
