public class Foo {
    void good() {
        SecretKeySpec secretKeySpec = new SecretKeySpec(Properties.getKey(), "AES");
    }

    void bad() {
        SecretKeySpec secretKeySpec = new SecretKeySpec("my secret here".getBytes(), "AES");
    }
}
