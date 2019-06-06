package com.example.tp1.models;

import org.json.JSONException;
import org.json.JSONObject;

public class User {
    private int id;
    private String email;
    private String tel;
    private String password;
    private String firstname;
    private String lastname;
    private String status;
    private int rights;
    private String lastSubscription;
    private String endSubscription;
    private String lastEdit;
    private String companyName;
    private int addressId;


    public User(JSONObject userJson){
        try {
            this.id = Integer.parseInt(userJson.getString("uid"));
            this.email = userJson.getString("email");
            this.tel = userJson.getString("tel");
            this.password = userJson.getString("password");
            this.firstname = userJson.getString("firstname");
            this.lastname = userJson.getString("lastname");
            this.status = userJson.getString("status");
            this.rights = Integer.parseInt(userJson.getString("rights"));
            this.lastSubscription = userJson.getString("lastSubscription");
            this.endSubscription = userJson.getString("endSubscription");
            this.lastEdit = userJson.getString("lastEdit");
            this.companyName = userJson.getString("companyName");
            if(userJson.getString("addressId")!= null){
                this.addressId = Integer.parseInt(userJson.getString("addressId"));
            }} catch (JSONException e){
            e.printStackTrace();
        }
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", email='" + email + '\'' +
                ", tel='" + tel + '\'' +
                ", password='" + password + '\'' +
                ", firstname='" + firstname + '\'' +
                ", lastname='" + lastname + '\'' +
                ", status='" + status + '\'' +
                ", rights=" + rights +
                ", lastSubscription='" + lastSubscription + '\'' +
                ", endSubscription='" + endSubscription + '\'' +
                ", lastEdit='" + lastEdit + '\'' +
                ", companyName='" + companyName + '\'' +
                ", addressId=" + addressId +
                '}';
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getTel() {
        return tel;
    }

    public void setTel(String tel) {
        this.tel = tel;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getFirstname() {
        return firstname;
    }

    public void setFirstname(String firstname) {
        this.firstname = firstname;
    }

    public String getLastname() {
        return lastname;
    }

    public void setLastname(String lastname) {
        this.lastname = lastname;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public int getRights() {
        return rights;
    }

    public void setRights(int rights) {
        this.rights = rights;
    }

    public String getLastSubscription() {
        return lastSubscription;
    }

    public void setLastSubscription(String lastSubscription) {
        this.lastSubscription = lastSubscription;
    }

    public String getEndSubscription() {
        return endSubscription;
    }

    public void setEndSubscription(String endSubscription) {
        this.endSubscription = endSubscription;
    }

    public String getLastEdit() {
        return lastEdit;
    }

    public void setLastEdit(String lastEdit) {
        this.lastEdit = lastEdit;
    }

    public String getCompanyName() {
        return companyName;
    }

    public void setCompanyName(String companyName) {
        this.companyName = companyName;
    }

    public int getAddressId() {
        return addressId;
    }

    public void setAddressId(int addressId) {
        this.addressId = addressId;
    }
}
