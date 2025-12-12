const std = @import("std");
const input = @embedFile("input.txt");

pub fn main() void {
    var width: u32 = 0;
    while (input[width] != '\n') width += 1;
    const stride: u32 = width + 1;

    const height: u32 = @as(u32, @intCast(input.len)) / stride;

    std.debug.print("Width: {}\n", .{width});
    std.debug.print("Height: {}\n", .{height});

    var sum: u64 = 0;

    var xStart: u32 = 0;
    while (xStart < width) {
        // Handle a single problem
        const op = input[(height-1) * stride + xStart];

        // Parse and apply each number
        var res: u64 = 0;
        var x: u32 = xStart;
        while (true) {
            // Parse this column
            var n: u32 = 0;
            var y: u32 = 0;
            while (y < height and input[y * stride + x] == ' ') y += 1;
            if (y == height) break;
            while (input[y*stride+x] >= '0' and input[y*stride+x] <= '9') {
                n = n * 10 + (input[y*stride+x] - '0');
                y += 1;
            }

            std.debug.print("n: {}\n", .{n});
            if (res == 0) {
                res = n;
            } else {
                if (op == '+') res += n;
                if (op == '*') res *= n;
            }
            x += 1;

            if (x >= width or input[(height-1)*stride+x] != ' ') break;
        }

        std.debug.print("res: {}\n", .{res});
        sum += res;

        xStart = x + 1;
    }

    std.debug.print("Sum: {}\n", .{sum});
}
