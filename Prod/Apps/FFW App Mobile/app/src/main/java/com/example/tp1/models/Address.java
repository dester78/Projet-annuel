package com.example.tp1.models;

public class Address {
    private int id;
    private String adress;
    private String complement;
    private String city;
    private String zipcode;
    private String country;

    public Address(String a, String com, String ci, String zc, String co, int id){
        this.adress = a;
        this.complement = com;
        this.city = ci;
        this.zipcode = zc;
        this.country = co;
        this.id = id;
    }
}
