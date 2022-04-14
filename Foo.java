public class Foo {
    void good() {
        SecureRandom random = new SecureRandom();
        byte iv[] = new byte[16];
        random.nextBytes(bytes);
    }

    void bad() {
        byte[] iv = new byte[] { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, };
    }

    void alsoBad() {
        byte[] iv = "secret iv in here".getBytes();
    }
}
