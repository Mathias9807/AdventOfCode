const fs = require('node:fs');

function isNumRepeated(n) {
  const nStr = '' + n;
  // Test all multiples between 1 and half of the length
  for (let mul = 2; mul <= nStr.length; mul++) {
    let match = true;
    const subLen = nStr.length / mul;
    if (subLen != Math.round(subLen)) continue;
    const first = nStr.substring(0, subLen);
    for (let i = 0; i < mul; i++) {
      if (nStr.substring(i * subLen, (i + 1) * subLen) != first) match = false;
    }
    if (match) {
      console.log('\t', n, subLen);
      return true;
    }
  }
  return false;
}

fs.readFile('input.txt', 'utf8', (err, data) => {
  if (err) {
    console.error(err);
    return;
  }

  const ranges = data
    .split(',')
    .map(r => r.trim().split('-'))
    .map(r => ({ start: +r[0], end: +r[1] }));

  let count = 0;
  let sum = 0;
  for (const r of ranges) {
    let start = '' + r.start;
    let end = '' + r.end;
    // if (start.length % 2 == 1) start = '1' + '0'.repeat(start.length);
    // if (end.length % 2 == 1) end = '9'.repeat(end.length - 1);
    // if (+start > +end) continue;

    console.log(start, end);

    // Iterate first half of range
    for (let i = +start; i <= +end; i++) {
      const iStr = '' + i;
      if (isNumRepeated(i)) {
        console.log(iStr);
        count++;
        sum += i;
      }
    }
  }

  console.log(`Count: ${count}`);
  console.log(`Sum: ${sum}`);
});
