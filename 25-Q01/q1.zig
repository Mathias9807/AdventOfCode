const std = @import("std");

const input = @embedFile("input.ex.txt");

pub fn _start() callconv(.naked) noreturn {
    asm volatile (
        \\ push %rbp
        \\ jmp %[start:P]
        :
        : [start] "X" (&main),
    );
}
comptime {
    @export(&_start, .{ .name = "_start" });
}

fn main() u8 {
    comptime var n: i32 = 50;
    comptime var pass: u32 = 0;

    comptime {
        var ptr: [*]u8 = @ptrCast(@constCast(input));
        @setEvalBranchQuota(100000);
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
            n += @as(i32, val) * @as(i32, @intCast(sign));
            ptr += 1;

            if (@mod(n, 100) == 0) pass += 1;
        }
    }

    // const max_len = 16;
    // var buf: [max_len]u8 = undefined;
    // const numAsString = std.fmt.bufPrint(&buf, "{}", .{pass}) catch unreachable;
    // std.fs.File.stdout().writeAll("Password: ") catch unreachable;
    // std.fs.File.stdout().writeAll(numAsString) catch unreachable;
    // std.fs.File.stdout().writeAll("\n") catch unreachable;

    // return @intCast(pass);
    std.posix.exit(@as(u8, @intCast(pass)));
}

// Compile small: zig build-exe -O ReleaseSmall -flto q1.zig && objcopy --remove-section=.comment --remove-section=.note q1
