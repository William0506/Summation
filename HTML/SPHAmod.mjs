function md(x, y, z) {
    return (...args) => args.slice(0, Math.pow(2, x + y + z));
}

function md1(x, y, z) {
    return function mul(a, b) {
        const n = x + y + z;
        if (n === 0) return [a[0] * b[0]];
        if (n === 1) {
            if (x === 1) return [a[0]*b[0] - a[1]*b[1], a[0]*b[1] + a[1]*b[0]];
            else if (y === 1) return [a[0]*b[0] + a[1]*b[1], a[0]*b[1] + a[1]*b[0]];
            else return [a[0]*b[0], a[0]*b[1] + a[1]*b[0]];
        }
        const halfLen = Math.pow(2, n - 1);
        const a1 = a.slice(0, halfLen), a2 = a.slice(halfLen);
        const b1 = b.slice(0, halfLen), b2 = b.slice(halfLen);
        let c1, c2;
        if (z > 0) {
            c1 = md1(x, y, z-1)(a1, b1);
            c2 = md1(x, y, z-1)(a1, b2).map((v, i) => v + md1(x, y, z-1)(a2, b1)[i]);
        } else if (y > 0) {
            c1 = md1(x, y-1, z)(a1, b1).map((v, i) => v + md1(x, y-1, z)(a2, b2)[i]);
            c2 = md1(x, y-1, z)(a1, b2).map((v, i) => v + md1(x, y-1, z)(a2, b1)[i]);
        } else {
            c1 = md1(x-1, y, z)(a1, b1).map((v, i) => v - md1(x-1, y, z)(a2, b2)[i]);
            c2 = md1(x-1, y, z)(a1, b2).map((v, i) => v + md1(x-1, y, z)(a2, b1)[i]);
        }
        return [...c1, ...c2];
    };
}

function md2(x, y, z) {
    return function div(a, b) {
        const n = x + y + z;
        if (n === 0) return [a[0] / b[0]];
        if (n === 1) {
            if (x === 1) {
                const denominator = b[0]**2 + b[1]**2;
                return [(a[0]*b[0] + a[1]*b[1]) / denominator, (a[1]*b[0] - a[0]*b[1]) / denominator];
            } else if (y === 1) {
                const denominator = b[0]**2 - b[1]**2;
                return [(a[0]*b[0] - a[1]*b[1]) / denominator, (a[1]*b[0] - a[0]*b[1]) / denominator];
            } else {
                return [a[0]*b[0] / b[0]**2, (a[1]*b[0] - a[0]*b[1]) / b[0]**2];
            }
        }
        const halfLen = Math.pow(2, n - 1);
        const b1 = b.slice(0, halfLen), b2 = b.slice(halfLen);
        const numerator = md1(x, y, z)(a, [...b1, ...b2.map(v => -v)]);
        let denominator;
        if (z > 0) {
            denominator = md1(x, y, z-1)(b1, b1);
            return [
                ...md2(x, y, z-1)(numerator.slice(0, halfLen), denominator),
                ...md2(x, y, z-1)(numerator.slice(halfLen), denominator)
            ];
        } else if (y > 0) {
            denominator = md1(x, y-1, z)(b1, b1).map((v, i) => v - md1(x, y-1, z)(b2, b2)[i]);
            return [
                ...md2(x, y-1, z)(numerator.slice(0, halfLen), denominator),
                ...md2(x, y-1, z)(numerator.slice(halfLen), denominator)
            ];
        } else {
            denominator = md1(x-1, y, z)(b1, b1).map((v, i) => v + md1(x-1, y, z)(b2, b2)[i]);
            return [
                ...md2(x-1, y, z)(numerator.slice(0, halfLen), denominator),
                ...md2(x-1, y, z)(numerator.slice(halfLen), denominator)
            ];
        }
    };
}

function ps(index, x, y, z) {
    if (index >= Math.pow(2, x + y)) return 0;
    const binary = index.toString(2).padStart(x + y + z, '0');
    const c_part = binary.slice(y + z);
    const ones_count = (c_part.match(/1/g) || []).length;
    return (ones_count % 2 === 0) ? 1 : -1;
}

function mp(x, y, z) {
    return function pow(a, b) {
        const n = x + y + z;
        let result = new Array(Math.pow(2, n)).fill(0);
        if (a !== 0) {
            const abs_a = Math.abs(a);
            const log_abs_a = Math.log(abs_a);
            if (a > 0) {
                result[0] = Math.pow(a, b[0]);
            } else {
                result[0] = Math.pow(-a, b[0]) * Math.cos(b[0] * Math.PI);
                result[Math.pow(2, n-1)] = Math.pow(-a, b[0]) * Math.sin(b[0] * Math.PI);
            }
            for (let i = 1; i < Math.pow(2, n); i++) {
                let term = new Array(Math.pow(2, n)).fill(0);
                const ps_i = ps(i, x, y, z);
                if (ps_i > 0) {
                    term[0] = Math.cosh(b[i] * log_abs_a);
                    term[i] = Math.sinh(b[i] * log_abs_a);
                } else if (ps_i < 0) {
                    term[0] = Math.cos(b[i] * log_abs_a);
                    term[i] = Math.sin(b[i] * log_abs_a);
                } else {
                    term[0] = 1;
                    term[i] = b[i] * log_abs_a;
                }
                result = md1(x, y, z)(result, term);
                if (a < 0) {
                    const rotationFactor = Math.exp(ps_i * Math.PI * b[i]);
                    result = result.map(v => v * rotationFactor);
                }
            }
        } else {
            if (b.every(v => v === 0)) {
                result[0] = 1;
            } else {
                result[0] = 0;
            }
        }
        return result;
    };
}
export { md, md1, md2, mp } ;
