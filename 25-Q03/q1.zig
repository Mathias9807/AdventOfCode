const std = @import("std");

const input = @embedFile("input.txt");

fn maxNum(bank: []const u8) u32 {
    var i: u32 = 0;
    var maxI: u32 = 0;
    while (i < bank.len - 1) {
        if (bank[i] > bank[maxI]) maxI = i;
        i += 1;
    }

    i = maxI + 1;
    var maxJ: u32 = i;
    while (i < bank.len) {
        if (bank[i] > bank[maxJ]) {
            maxJ = i;
        }
        // std.debug.print("i: {}, bank[i]: {}, maxJ: {}\n", .{i, bank[i] - '0', maxJ});
        i += 1;
    }

    return @as(u32, @intCast(bank[maxI] - '0')) * 10 + @as(u32, @intCast(bank[maxJ] - '0'));
}

pub fn main() void {
    var total: u32 = 0;
    var start: u32 = 0;
    while (start < input.len) {
        var i: u32 = start;
        while (i < input.len and input[i] != '\n') {
            i += 1;
        }

        const slice = input[start..i];
        total += maxNum(slice);
        std.debug.print("Num {s} {}\n", .{slice, maxNum(slice)});

        start = i + 1;
    }

    std.debug.print("Total {}\n", .{total});
}
