/// <reference path='../_app.ts' />

/**
 * Created by omnia on 11/16/15.
 */

module RobotArm {
    export class indexController {
        public text: string;

        //noinspection JSUnusedGlobalSymbols
        public static $inject = ['$location', '$mdSidenav', '$log'];

        constructor(private location: ng.ILocationService, private mdSideNav: any, private log: ng.ILogService) {
            this.text = "Sample Text"
        }

        public toggleSidePane() {
            this.mdSideNav('left')
                .toggle()
                .then(function () {
                    this.log.debug("Toggled Left is Done");
                });
        }
    }

    angular.module('RobotArm').controller('IndexController', indexController);
}