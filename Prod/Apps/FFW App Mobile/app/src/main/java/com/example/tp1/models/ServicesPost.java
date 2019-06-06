package com.example.tp1.models;

public class ServicesPost {
    private String title;
    private String content;
    private int comments;
    private String author;

    public ServicesPost(String t, String c, String a){
        this.title = t;
        this.content = c;
        this.author = a;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public int getComments() {
        return comments;
    }

    public void setComments(int comments) {
        this.comments = comments;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }
}
