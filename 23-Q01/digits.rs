use std::fs::File;
use std::io::prelude::*;
use std::path::Path;

fn main() {
	let path = Path::new("input.txt");
	let display = path.display();

	let mut file = match File::open(&path) {
		Err(why) => panic!("couldn't open {}: {}", display, why),
		Ok(file) => file
	};

	let mut s = String::new();
	match file.read_to_string(&mut s) {
	    Err(why) => panic!("couldn't read {}: {}", display, why),
	    Ok(_) => print!("{} contains:\n{}", display, s),
	}

	let mut sum = 0;
	let mut first_digit: i32 = -1;
	let mut last_digit: i32 = -1;
	for c in s.chars() {
		if c.is_digit(10) {
			if first_digit == -1 {
				first_digit = c.to_digit(10).expect("error") as i32;
			}

			last_digit = c.to_digit(10).expect("error") as i32;
		}

		if c == '\n' {
			sum += first_digit * 10 + last_digit;
			first_digit = -1;
			last_digit = -1;
		}
	}

	print!("Result: {}\n", sum);
}

