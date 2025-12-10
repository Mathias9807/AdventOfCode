const std = @import("std");
const input = @embedFile("input.txt");

pub fn main() void {
    var width: usize = 0;
    while (input[width] != '\n') width += 1;

    const stride = width + 1;

    const height: usize = input.len / stride;

    var total: usize = 0;

    var x: usize = 0;
    while (x < width) {
        defer x += 1;
        var y: usize = 0;
        while (y < height) {
            defer y += 1;

            // std.debug.print("x: {}, y: {}, stride: {}\n", .{x, y, stride});
            if (input[y * stride + x] != '@') continue;

            // Test roll
            var num_surr_rolls: u32 = 0;
            var dx: i32 = -1;
            while (dx <= 1) {
                var dy: i32 = -1;
                while (dy <= 1) {
                    defer dy += 1;
                    if (dx == 0 and dy == 0) continue;

                    const X = @as(i32, @intCast(x)) + dx;
                    const Y = @as(i32, @intCast(y)) + dy;
                    if (X >= 0 and X < width and Y >= 0 and Y < height) {
                        if (input[@as(usize, @intCast(Y * @as(i32, @intCast(stride)) + X))] == '@')
                            num_surr_rolls += 1;
                    }
                }
                dx += 1;
            }

            if (num_surr_rolls < 4) total += 1;
        }
    }

    std.debug.print("Num rolls: {}\n", .{total});
}
