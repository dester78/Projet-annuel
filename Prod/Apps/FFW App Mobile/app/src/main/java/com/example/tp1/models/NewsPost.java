package com.example.tp1.models;

import com.google.gson.JsonObject;

import org.json.JSONException;
import org.json.JSONObject;

public class NewsPost {
    private String title;
    private String content;
    private int comments;
    private String author;

    public NewsPost(String t, String c, String a){
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

   // public NewsPost(JSONObject newsJson){
   //     try {
   //         this.id = Integer.parseInt(newsJson.getString("uid"));
   //         this.date = newsJson.getString("date");
   //         this.content = newsJson.getString("content");
   //     } catch (JSONException e){
   //         e.printStackTrace();
   //     }
   // }
}
