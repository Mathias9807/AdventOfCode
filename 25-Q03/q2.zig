const std = @import("std");

const input = @embedFile("input.txt");

const digits = 12;

fn maxNum(bank: []const u8) u64 {
    var total: u64 = 0;

    var start: usize = 0;
    var digit: usize = 0;
    while (digit < digits) {
        var i: usize = start;
        var maxI: usize = start;
        while (i < bank.len - (digits - digit - 1)) {
            if (bank[i] > bank[maxI]) maxI = i;
            i += 1;
        }
        total = total * 10 + @as(u64, @intCast(bank[maxI] - '0'));
        start = maxI + 1;
        digit += 1;
    }

    return total;
}

pub fn main() void {
    var total: u64 = 0;
    var start: u64 = 0;
    while (start < input.len) {
        var i: u64 = start;
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
