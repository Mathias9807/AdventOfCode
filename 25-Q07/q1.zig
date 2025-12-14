const std = @import("std");
var arenaAlloc = std.heap.ArenaAllocator.init(std.heap.page_allocator);
const alloc = arenaAlloc.allocator();

const input = @embedFile("input.txt");
var map: [][]u8 = undefined;

var width: u32 = 0;
var height: u32 = 0;

var total: u32 = 0;

pub fn printMap() void {
    std.debug.print("\x1B[2J\x1B[H", .{});
    for (0..height) |y| {
        std.debug.print("{s}\n", .{map[y]});
    }
    std.debug.print("\n", .{});
}

const Beam = struct {
    x: u32, y: u32,

    pub fn init(x: u32, y: u32) !Beam {
        if (x < 0 or x >= width or y < 0 or y >= height)
            return error.Overflow;
        if (map[y][x] == '|')
            return error.Overflow;
        map[y][x] = '|';
        return .{ .x = x, .y = y };
    }

    pub fn tick(self: *Beam) void {
        while (self.y + 1 < height) {
            const next = map[self.y + 1][self.x];
            if (next == '^') {
                if (Beam.init(self.x - 1, self.y + 1)) |beam| {
                    var b = beam;
                    b.tick();
                } else |_| {}
                if (Beam.init(self.x + 1, self.y + 1)) |beam| {
                    var b = beam;
                    b.tick();
                } else |_| {}
                total += 1;
                break;
            } else if (next == '.') {
                self.y += 1;
                map[self.y][self.x] = '|';
            } else {
                // Hit another beam
                break;
            }
        }
        // printMap();
    }
};

pub fn main() void {
    while (input[width] != '\n') width += 1;
    const stride: u32 = width + 1;
    height = @as(u32, @intCast(input.len)) / stride;

    map = alloc.alloc([]u8, height) catch unreachable;
    for (0..height) |i| {
        map[i] = alloc.alloc(u8, width) catch unreachable;
        @memcpy(map[i], input[i * stride..i * stride + width]);
    }

    var S: u32 = 0;
    for (0..width) |i| {
        if (map[0][i] == 'S') S = @as(u32, @intCast(i));
    }

    var b = Beam.init(S, 0) catch unreachable;
    b.tick();

    std.debug.print("Total: {}\n", .{total});
}
