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
    rangeLoop: while (true) {
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
        i += 1;

        for (ranges[0..nRanges], 0..) |r, rI| {
            if (range.start >= r.start and range.end <= r.end) continue :rangeLoop;
            if (range.start < r.start and range.end > r.end) {
                // Expand the existing range
                ranges[rI].start = range.start;
                ranges[rI].end = range.end;
                continue :rangeLoop;
            }
            if (range.start >= r.start and range.start <= r.end) range.start = r.end + 1;
            if (range.end >= r.start and range.end <= r.end) range.end = r.start - 1;
        }

        if (range.start <= range.end) {
            ranges[nRanges] = range;
            nRanges += 1;
        }
    }
    i += 1;

    var nFresh: u64 = 0;
    for (ranges[0..nRanges]) |r| {
        std.debug.print("Range: {}-{}\n", .{r.start, r.end});
        nFresh += r.end - r.start + 1;
    }

    std.debug.print("Number of fresh vegetables: {}\n", .{nFresh});
}
