package com.github.amanganiello90.javafullstack.app.services;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;

public class ErrorTests {
    private String str;
    private final int testInt;

    public ErrorTests(String str, int testInteger){
        this.str = str;
        this.testInt = testInteger;
    }

    public void testMethod(){
        Runtime runtime = Runtime.getRuntime();

        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }

        try (ByteArrayOutputStream stdout = new ByteArrayOutputStream();
                ByteArrayOutputStream stderr = new ByteArrayOutputStream()) {
            stdout.close();
            stderr.close();
            System.out.println(stdout.toString());
        } catch (IOException e) {
            throw new RuntimeException("command: ", e);
        }


        File fPath = new File("");
        if (fPath.isDirectory()) {
            File[] langPaths = fPath.listFiles();
            if (langPaths.length > 0) {
                System.out.println("null pointer problem");
            }
        }

    }
public void testMethod(){
        Runtime runtime = Runtime.getRuntime();

        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }

        try (ByteArrayOutputStream stdout = new ByteArrayOutputStream();
                ByteArrayOutputStream stderr = new ByteArrayOutputStream()) {
            stdout.close();
            stderr.close();
            System.out.println(stdout.toString());
        } catch (IOException e) {
            throw new RuntimeException("command: ", e);
        }


        File fPath = new File("");
        if (fPath.isDirectory()) {
            File[] langPaths = fPath.listFiles();
            if (langPaths.length > 0) {
                System.out.println("null pointer problem");
            }
        }

    }
    public void testMethod1(){
        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }



        ByteArrayOutputStream stdout = null;
        ByteArrayOutputStream stderr = null;
        try {
            stdout = new ByteArrayOutputStream();
            stderr = new ByteArrayOutputStream();
            try {
                stdout.close();
                stderr.close();
                System.out.println(stdout.toString());
            } catch (IOException e) {
                throw new RuntimeException("command: ", e);
            }
        } finally {
            try {
                if (stdout != null) {
                    stdout.close();
                }
                if (stderr != null) {
                    stderr.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }
    }
}

public class ErrorTests {
    private String str;
    private final int testInt;

    public ErrorTests(String str, int testInteger){
        this.str = str;
        this.testInt = testInteger;
    }

    public void testMethod(){
        Runtime runtime = Runtime.getRuntime();

        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }

        try (ByteArrayOutputStream stdout = new ByteArrayOutputStream();
                ByteArrayOutputStream stderr = new ByteArrayOutputStream()) {
            stdout.close();
            stderr.close();
            System.out.println(stdout.toString());
        } catch (IOException e) {
            throw new RuntimeException("command: ", e);
        }


        File fPath = new File("");
        if (fPath.isDirectory()) {
            File[] langPaths = fPath.listFiles();
            if (langPaths.length > 0) {
                System.out.println("null pointer problem");
            }
        }

    }
public void testMethod(){
        Runtime runtime = Runtime.getRuntime();

        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }

        try (ByteArrayOutputStream stdout = new ByteArrayOutputStream();
                ByteArrayOutputStream stderr = new ByteArrayOutputStream()) {
            stdout.close();
            stderr.close();
            System.out.println(stdout.toString());
        } catch (IOException e) {
            throw new RuntimeException("command: ", e);
        }


        File fPath = new File("");
        if (fPath.isDirectory()) {
            File[] langPaths = fPath.listFiles();
            if (langPaths.length > 0) {
                System.out.println("null pointer problem");
            }
        }

    }
    public void testMethod1(){
        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }



        ByteArrayOutputStream stdout = null;
        ByteArrayOutputStream stderr = null;
        try {
            stdout = new ByteArrayOutputStream();
            stderr = new ByteArrayOutputStream();
            try {
                stdout.close();
                stderr.close();
                System.out.println(stdout.toString());
            } catch (IOException e) {
                throw new RuntimeException("command: ", e);
            }
        } finally {
            try {
                if (stdout != null) {
                    stdout.close();
                }
                if (stderr != null) {
                    stderr.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }


        System.out.println(str);
        if (str.startsWith("test") == true) {
            if(testInt == 0) {
                System.out.println(testInt + str);
            }
        }
    }
}
