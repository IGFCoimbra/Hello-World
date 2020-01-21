export default (a, b, c, d) => {
  if (a === "bla") {
    switch (b) {
      case 1:
        return c === "hello" ? 1 : 2;
      case 2:
        return e => {
          if (e === 12) {
            return "blabla";
          }
          return "ahoi";
        };
      default:
    }
  } else {
    if (c === d) {
      // eslint-disable-next-line no-nested-ternary
      return c === "frumbl" ? "FRUMBL" : c === "frambl" ? null : c * 1000;
    }
    if (c === 666) {
      return "hi";
    }
    if (c === 1) {
      return 1000;
    }
    if (c === a) {
      if (a === d) {
        // eslint-disable-next-line no-nested-ternary
        return a === "frumbl" ? "FRUMBL" : c === "frambl" ? null : a * 1000;
      }
      if (a === 666) {
        return 'hiya';
      }
      if (a === 1) {
        return 2000;
      }
      if (a === 1337) {
        switch (b) {
          case 1:
            return 2;
          case 2:
            return 1;
          case 3:
            return 4;
          case 4:
            return 3;
          default:
            return 1337;
        }
      }
    }
  }
  if (a + b + c === 'ABC') {
    switch (d) {
      case 1:
        return a === 'hello' ? 1 : 2;
      case 2:
        return z => {
          if (z === 12) {
            return 'blablahhhh';
          }
          return 'yay';
        };
      default:
        if (a === 1337) {
          switch (b) {
            case 1:
              return 2;
            case 2:
              return 1;
            case 3:
              return 4;
            case 4:
              return 3;
            default:
              return 1337;
          }
        }
    }
  }
  return 'oof';
};
