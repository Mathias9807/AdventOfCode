const std = @import("std");

const input = @embedFile("input.q1.txt");

pub fn main() void {
    var n: i32 = 50;
    var pass: u32 = 0;

    var ptr: [*]u8 = @ptrCast(@constCast(input));
    while (ptr[0] != 0) {
        var sign: i8 = 1;
        if (ptr[0] == 'L') sign = -1;
        ptr += 1;
        var val = @as(i32, @intCast(ptr[0] - '0'));
        ptr += 1;
        while (ptr[0] != '\n') {
            val = val * 10 + ptr[0] - '0';
            ptr += 1;
        }
        ptr += 1;

        // Reset n to between 0-99 range
        n = @mod(n, 100);
        std.debug.print("n: {}\n", .{n});
        const prev_n = n; // 85
        // Calc offset
        var offset = @as(i32, val) * @as(i32, @intCast(sign)); // -425
        std.debug.print("offset: {}\n", .{offset});
        // Tick for every loop (100)
        const cycles = @divTrunc(offset, 100); // -4
        pass += @abs(cycles);
        offset -= cycles * 100;
        // Change n
        n += offset; // n = -340
        // Tick if landed on 0
        if (n == 0) pass += 1;
        // Tick if < 0 or > 99
        if ((prev_n != 0 and n < 0) or n > 99) pass += 1;
        std.debug.print("ticks: {}\n", .{pass});
    }

    std.debug.print("Password: {}\n", .{pass});
}
