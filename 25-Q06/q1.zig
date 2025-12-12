const std = @import("std");
const input = @embedFile("input.txt");

pub fn main() void {
    var nums: [1000]u64 = undefined;
    var numLen: u32 = 0;

    var ops: [1000]u8 = undefined;
    var opLen: u32 = 0;

    // Pick out the operators
    var i: u32 = 0;
    while (i < input.len) {
        while (input[i] == ' ') i += 1;
        if (input[i] >= '0' and input[i] <= '9') {
            while (input[i] != '\n') {
                i += 1;
            }
            i += 1;
        } else {
            while (input[i] != '\n') {
                ops[opLen] = input[i];
                opLen += 1;
                i += 1;
                while (input[i] == ' ') i += 1;
            }
            i += 1;
        }
    }

    // Read the numbers
    i = 0;
    while (i < input.len) {
        std.debug.print("i: {}, c: {c}\n", .{i, input[i]});
        while (input[i] == ' ') i += 1;
        if (input[i] >= '0' and input[i] <= '9') {
            var nI: u32 = 0;
            while (input[i] != '\n') {
                var n: u32 = 0;
                while (input[i] >= '0' and input[i] <= '9') {
                    n = n * 10 + (input[i] - '0');
                    i += 1;
                }
                while (input[i] == ' ') i += 1;

                if (nI < numLen) {
                    if (ops[nI] == '+') {
                        nums[nI] += n;
                    } else if (ops[nI] == '*') {
                        nums[nI] *= n;
                    }
                } else {
                    nums[numLen] = n;
                    numLen += 1;
                }
                nI += 1;
            }
            i += 1;
        } else break;
    }

    var sum: u64 = 0;
    for (nums[0..numLen]) |n| {
        sum += n;
    }

    std.debug.print("Sum: {}\n", .{sum});
}
