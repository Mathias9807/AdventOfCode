const std = @import("std");
const input = @embedFile("input.txt");

const Range = struct {
    start: u64,
    end: u64,
};

pub fn main() void {
    var ranges: [10000]Range = undefined;
    var nRanges: u32 = 0;

    var i: u32 = 0;
    while (true) {
        if (input[i] == '\n') break;

        // Read start of range
        var range: Range = .{ .start = 0, .end = 0 };
        while (input[i] != '-') {
            range.start = range.start * 10 + (input[i] - '0');
            i += 1;
        }
        i += 1;
        // Read end
        while (input[i] != '\n') {
            range.end = range.end * 10 + (@as(u32, @intCast(input[i])) - '0');
            i += 1;
        }

        ranges[nRanges] = range;
        nRanges += 1;

        i += 1;
    }
    i += 1;

    var nFresh: u32 = 0;
    var nVegs: u32 = 0;
    while (input[i] >= '0' and input[i] <= '9' and i < input.len) {
        var n: u64 = 0;
        while (input[i] != '\n') {
            n = n * 10 + (input[i] - '0');
            i += 1;
        }

        var j: u32 = 0;
        while (j < nRanges) {
            if (n >= ranges[j].start and n <= ranges[j].end) {
                std.debug.print("Found fresh {}\n", .{nVegs});
                nFresh += 1;
                break;
            }
            j += 1;
        }

        nVegs += 1;
        i += 1;
    }

    std.debug.print("Number of fresh vegetables: {}\n", .{nFresh});
}
