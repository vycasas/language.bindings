package net.dotslashzero.javalib;

/**
 * General library operations.
 */
public class Library
{
    public static void initialize() throws UnsatisfiedLinkError
    {
        System.loadLibrary("javalib");
        return;
    }

    public static void terminate()
    {
        return;
    }

    private Library()
    {
    }
}